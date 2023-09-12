#ifndef _IMG_TR_FUNCTIONS_HPP_
    #define _IMG_TR_FUNCTIONS_HPP_
    
#include <stdint.h>

void copying(unsigned char *img, unsigned char *same_img, size_t img_size);
void monochromating(unsigned char *img, unsigned char *gray_img, size_t img_size);
void edging(unsigned char *img, unsigned char *edge_img, int width, int height, size_t img_size);

#endif
