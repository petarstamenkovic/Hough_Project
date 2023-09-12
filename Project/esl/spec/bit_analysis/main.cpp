#include <systemc>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "img_tr_functions.hpp"
#include "hough_functions.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

int sc_main(int argc, char *argv[])
{
    dw_t draw_factor;
    unsigned char *img;
    unsigned char *result_img;
    unsigned char *gray_img;
    unsigned char *edge_img;
    int width, height, channels;
    size_t gray_img_size;
    size_t img_size;
    int16_t acc[THETA2][RHO];
    int16_t acc_max;
    int i;
    char result_image[] = "result_img/i.jpg";
    
    for (i = 0; i < argc-2; i+=2)
    {
	    draw_factor = atof(argv[i+1]);
	    img = stbi_load(argv[i+2], &width, &height, &channels, 0);
	    if (img == NULL)
	    {
		printf("Greska prilikom ucitavanja fotografije.\n");
		exit(1);
	    }

	    gray_img_size = width * height;
	    img_size = gray_img_size * channels;

	    result_img = (unsigned char *)malloc(img_size);
	    if(result_img == NULL)
	    {
		printf("Nije moguca alokacija memorije za rezultujucu fotografiju.\n");
		exit(1);
	    }

	    gray_img = (unsigned char *)malloc(gray_img_size);
	    if(gray_img == NULL)
	    {
		printf("Nije moguca alokacija memorije za monohromatsku fotografiju.\n");
		exit(1);
	    }

	    edge_img = (unsigned char *)malloc(gray_img_size);
	    if(edge_img == NULL)
	    {
		printf("Nije moguca alokacija memorije za fotografiju sa izdvojenim ivicama.\n");
		exit(1);
	    }
	    
	    copying(img, result_img, width, height, img_size);
	    monochromating(img, gray_img, width, height, img_size);
	    edging(gray_img, edge_img, width, height, gray_img_size);
	    
	    acc_init(acc);
	    hough(edge_img, acc, width, height, THRESHOLD);
	    acc_max = max_acc(acc);
	    
	    draw_lines(result_img, acc, width, height, acc_max, draw_factor);
	    
	    result_image[11] = ((i/2) % 10) + 48;
	    if (i%20 == 0) result_image[10]++;
	    stbi_write_jpg(result_image, width, height, channels, result_img, 100);

	    free(gray_img);
	    free(edge_img);
	    stbi_image_free(img);
	    free(result_img);
	    
	    printf("ZAVRSENO!\n");
    }
    
    return 0;
}
