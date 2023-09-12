#include <cmath>
#include "bit_analysis/img_tr_functions.hpp"

void monochromating(unsigned char *img, unsigned char *gray_img, size_t img_size)
{
    unsigned char *p;
    unsigned char *pg;

    for(p = img, pg = gray_img; p != img + img_size; p += 3, pg++)
    {
        *pg = (uint8_t) ((*p + *(p + 1) + *(p + 2))/3.0);
    }
}

void copying(unsigned char *img, unsigned char *same_img, size_t img_size)
{
    unsigned char *p;
    unsigned char *pg;

    for(p = img, pg = same_img; p != img + img_size; p++, pg++)
    {
        *pg = *p;
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
