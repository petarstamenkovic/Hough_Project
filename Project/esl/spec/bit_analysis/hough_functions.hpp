#ifndef _HOUGH_FUNCTIONS_HPP_
    #define _HOUGH_FUNCTIONS_HPP_
    
#include "types.hpp"
    
#define THETA 180
#define THRESHOLD 149
#define THETA2 270
#define RHO 2047

void acc_init(int16_t acc[THETA2][RHO]);
void hough(unsigned char *img, int16_t acc[THETA2][RHO], int width, int height, uint8_t transform_threshold);
int16_t max_acc(int16_t acc[THETA2][RHO]);
void draw_lines(unsigned char *img, int16_t acc[THETA2][RHO], int width, int height, int16_t acc_max, dw_t draw_factor);

#endif
