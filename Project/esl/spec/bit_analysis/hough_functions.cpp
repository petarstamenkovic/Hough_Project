#include <cmath>
#include "hough_functions.hpp"
#include "types.hpp"

void acc_init(int16_t acc[THETA2][RHO])
{
    int16_t i, j;

    for (i = 0; i < THETA2; i++)
    {
        for (j = 0; j < RHO; j++)
        {
            acc[i][j] = 0;
        }
    }
}

void hough(unsigned char *img, int16_t acc[THETA2][RHO], int width, int height, uint8_t transform_threshold)
{
    unsigned char *p;
    int16_t x, y;
    int16_t theta, rho;
    trig_t sin_pom, cos_pom;

    for (p = img, y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++, p++)
        {
            if (*p > transform_threshold)
            {
                for (theta = 0; theta < THETA; theta++)
                {
                    sin_pom = sin(theta*M_PI/180);
                    cos_pom = cos(theta*M_PI/180);
                    rho = x*sin_pom - y*cos_pom;
                    if (rho >= 0)
                    {
                        acc[theta][rho]++;
                    }
                    else
                    {
                        acc[THETA + theta][-rho]++;
                    }
                    
                }
            }
        }
    }
}

int16_t max_acc(int16_t acc[THETA2][RHO])
{
    int16_t i, j;
    int16_t acc_max;

    for (i = 0; i < THETA2; i++)
    {
        for (j = 0; j < RHO; j++)
        {
            if (acc[i][j] > acc_max)
            {
                acc_max = acc[i][j];
            }
        }
    }

    return acc_max;
}

void draw_lines(unsigned char *img, int16_t acc[THETA2][RHO], int width, int height, int16_t acc_max, dw_t draw_factor)
{
    int16_t acc_tres;
    int16_t theta, rho;
    trig_t a, b;
    point_t x0, x1, y0, y1;
    slope_t slope;
    intercept_t intercept;
    int16_t x, y;
    int16_t y_new, y_old = 0;

    acc_tres = draw_factor*acc_max;

    for (rho = 0; rho < RHO; rho++)
    {
        for (theta = 0; theta < THETA2; theta++)
        {
            if (acc[theta][rho] >= acc_tres) 
            {
               if (theta >= THETA)
                {
                    a = cos((theta-THETA)*M_PI/180);
                    b = sin((theta-THETA)*M_PI/180);
                    x0 = - b * rho;
                    y0 = a * rho;
                }
                else
                {
                    a = cos(theta*M_PI/180);
                    b = sin(theta*M_PI/180);
                    x0 = b * rho;
                    y0 = - a * rho;
                }

                x1 = x0 + 10*a;
                y1 = y0 + 10*b;
                
                if (a == 0) 
                    slope = height;
                else 
                    slope = b/a;
                    
                intercept = y1 - slope * x1;

                //printf("x0 = %lf, y0 = %lf, theta = %d, rho = %d, slope = %lf, intercept = %lf\n", x0, y0, theta, rho, slope, intercept);

                if (slope >= height || slope <= -height)
                {
                    for (y = 0; y < height; y++)
                    {
                        *(img + 3*(y*width + rho)) = 255;
                        *(img + 3*(y*width + rho) + 1) = 0;
                        *(img + 3*(y*width + rho) + 2) = 0; 
                    }
                }
                else
                {
                    for (x = 0; x < width; x++) 
                    {
                        y = (int)(slope * x + intercept);
                        y_new = y;
                        
                        // printf("x = %d,  y = %d \n", x, y);
                        if (y >= 0 && y < height) 
                        {
                            if (y_new > y_old + 1 && y_old >= 0 && x != 0 && slope > 0) 
                            {
                                for(y = y_old + 1; y < y_new; y++)
                                {
                                    *(img + 3*(y*width + x)) = 255;
                                    *(img + 3*(y*width + x) + 1) = 0;
                                    *(img + 3*(y*width + x) + 2) = 0;
                                }
                            }
                            else if (y_new < y_old - 1 && y_old < height && x != 0 && slope < 0) 
                            {
                                for(y = y_old - 1; y > y_new; y--)
                                {
                                    *(img + 3*(y*width + x)) = 255;
                                    *(img + 3*(y*width + x) + 1) = 0;
                                    *(img + 3*(y*width + x) + 2) = 0;
                                }
                            }
                            
                            *(img + 3*(y*width + x)) = 255;
                            *(img + 3*(y*width + x) + 1) = 0;
                            *(img + 3*(y*width + x) + 2) = 0;
                        }
                        else if (y_old > 0 && y_old < height && slope > 0)
                        {
                            for(y = y_old; y < height; y++)
                            {
                                *(img + 3*(y*width + x)) = 255;
                                *(img + 3*(y*width + x) + 1) = 0;
                                *(img + 3*(y*width + x) + 2) = 0;
                            }
                        }
                        else if (y_old > 0 && y_old < height && slope < 0)
                        {
                            for(y = y_old; y >= 0; y--)
                            {
                                *(img + 3*(y*width + x)) = 255;
                                *(img + 3*(y*width + x) + 1) = 0;
                                *(img + 3*(y*width + x) + 2) = 0;
                            }
                        }

                        y_old = y_new;
                    }
                }
                
                y_new = 0;
                y_old = 0;
            }
        }
    }
}
