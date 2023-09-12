#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#define RHO 442
#define HALF_THETA 135

void monochromating(unsigned char *img, unsigned char *gray_img, size_t img_size);
void edging(unsigned char *img, unsigned char *edge_img, int width, int height, size_t img_size);
	

int main(int argc, char *argv[])
{	
	unsigned char *img;
	unsigned char *gray_img;
	unsigned char *edge_img;
	int width, height, channels;
	int16_t rho;
	size_t gray_img_size;
	size_t img_size;
	int i;
	unsigned char result_file[] = "case_img_.txt";
	FILE *fp;
	unsigned char *tp;
    
	for (i = 0; i < argc - 1; i++)
	{
		img = stbi_load(argv[i+1], &width, &height, &channels, 0);
		if (img == NULL)
		{
			printf("Greska prilikom ucitavanja fotografije.\n");
			exit(1);
		}

		gray_img_size = width * height;
		img_size = gray_img_size * channels;
		
		rho = (int16_t) sqrt(width*width + height*height);
		if (rho > RHO)
		{
			printf("Uneli ste preveliku fotografiju (dijagonala = %d)!\n NAJVECA DOZVOLJENA VREDNOST JE %d\n)", rho, RHO);
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
		
		monochromating(img, gray_img, img_size);
		edging(gray_img, edge_img, width, height, gray_img_size);
		

		result_file[8] = ((i) % 10) + 48;
		//if (i%10 == 0) result_file[3]++;
		
		fp = fopen(result_file, "w");
		if (fp == NULL)
		{
			printf("Greska prilikom otvaranja fajla!");
			return 0;
		}
		
		fprintf(fp, "#define WIDTH_C %d\n", width);
		fprintf(fp, "#define HEIGHT_C %d\n", height);
		fprintf(fp, "#define RHO_C %d\n\n", rho);
		fprintf(fp, "u32  case_img[] =\n");
		fprintf(fp, "{\n");
		for(tp = edge_img; tp != edge_img + gray_img_size; tp++)
		{
			fprintf(fp, "%d, ", *tp);
			if ((tp - edge_img + 1) % 50 == 0)
				fprintf(fp, "\n");
		}
		fprintf(fp, "\n};\n\n");
		
		fprintf(fp, "u32 acc0[] =\n");
		fprintf(fp, "{\n");
		for(i = 0; i != rho * HALF_THETA; i++)
		{
			fprintf(fp, "%d, ", 0);
			if ((i + 1) % 50 == 0)
				fprintf(fp, "\n");
		}
		fprintf(fp, "\n};\n\n");
		
		fprintf(fp, "u32 acc1[] =\n");
		fprintf(fp, "{\n");
		for(i = 0; i != rho * HALF_THETA; i++)
		{
			fprintf(fp, "%d, ", 0);
			if ((i + 1) % 50 == 0)
				fprintf(fp, "\n");
		}
		fprintf(fp, "};");
		
		fclose(fp);
		printf("DONE!\n");
		
		stbi_image_free(img);
		free(gray_img);
		free(edge_img);
	}
	
	return 0;
}

void monochromating(unsigned char *img, unsigned char *gray_img, size_t img_size)
{
    unsigned char *p;
    unsigned char *pg;

    for(p = img, pg = gray_img; p != img + img_size; p += 3, pg++)
    {
        *pg = (uint8_t) ((*p + *(p + 1) + *(p + 2))/3.0);
    }
}

void edging(unsigned char *img, unsigned char *edge_img, int width, int height, size_t img_size)
{
    unsigned char *p;
    unsigned char *pg;
    int16_t x = 0, y = 0;
    int16_t sum1, sum2;
    int8_t kernel_i, kernel_j;
    int8_t edge_matrix1[3][3] = {{-1, 0, 1},
                                {-2, 0, 2},
                                {-1, 0, 1}};
    int8_t edge_matrix2[3][3] = {{1, 2, 1},
                                {0, 0, 0},
                                {-1, -2, -1}};

    for(p = img, pg = edge_img; p != img + img_size; p ++, pg++)
    {
        sum1 = 0;
        sum2 = 0;

        if (!(x == 0 || y == 0 || x == width - 1 || y == height -1))
        {
            for (kernel_i = -1; kernel_i <= 1; kernel_i += 1)
            {
                for (kernel_j = -1; kernel_j <= 1; kernel_j++)
                {
                    sum1 += *(p + kernel_j + kernel_i*width) * edge_matrix1[kernel_i+1][kernel_j+1];
                    sum2 += *(p + kernel_j + kernel_i*width) * edge_matrix2[kernel_i+1][kernel_j+1];
                }
            }

            *pg = (uint8_t) sqrt(pow(sum1, 2.0) + pow(sum2, 2.0));
        }
        else
        {
            *pg = (uint8_t) 0;
        }
        
        x++;
        if(x == width)
        {
            x = 0;
            y++;
        }
    }
}
