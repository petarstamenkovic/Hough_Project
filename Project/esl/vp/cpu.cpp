#include <cmath>
#include "cpu.hpp"
#include "types.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

char** Cpu::data_string = nullptr;
int Cpu::argc = 0;

int write_count = 0;
int read_count = 0;

SC_HAS_PROCESS(Cpu);

Cpu::Cpu(sc_core::sc_module_name name, char** strings, int argv) : sc_module(name), offset(sc_core::SC_ZERO_TIME)
{
	SC_THREAD(cpu);
	SC_REPORT_INFO("Cpu", "Constructed.");
	data_string = strings;
	argc = argv;
}

Cpu::~Cpu()
{
    SC_REPORT_INFO("Cpu", "Destroyed.");
}

void Cpu::cpu()
{	
	dw_t draw_factor;
	unsigned char *img;
	unsigned char *result_img;
	unsigned char *gray_img;
	unsigned char *edge_img;
	int width, height, channels;
	int16_t rho;
	size_t gray_img_size;
	size_t img_size;
	unsigned char *acc0;
	unsigned char *acc1;
	size_t acc_size;
	unsigned char acc_max;
	int i, j;
	unsigned char result_image[] = "result_img/i.jpg";
	FILE *acc0_fp;
	FILE *acc1_fp;
	FILE *frame_fp;
	FILE *img_fp;
	FILE *dim_fp;
	char acc0_file[] = "acc0_vp.txt";
	char acc1_file[] = "acc1_vp.txt";
	char frame_file[] = "frame_rate_file.txt";
	char img_file[] = "img_file.txt";
	char dim_file[] = "dim_file.txt";
	long long offset_value_new = 0;
	long long offset_value_old = 0;
	long long offset_value = 0;
	long long offset_bw = 0;
	long long offset_bp = 0;
	long long offset_ap = 0;
	long long offset_ar = 0;
	double fps = 0;
	bool ready = 1;
	
	frame_fp = fopen((const char*) frame_file, "w");
	if (frame_fp == NULL)
	{
		printf("Greska prilikom otvaranja fajla za frame_rate!");
		return;
	}
	
	fprintf(frame_fp, "----- HOUGH VIRTUAL PLATFORM -----\n");
    
	for (i = 0; i < argc-2; i+=2)
	{
		draw_factor = atof(data_string[i+1]);
		img = stbi_load(data_string[i+2], &width, &height, &channels, 0);
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
		
		acc_size = THETA2*rho;

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
		
		acc0 = (unsigned char *)malloc(acc_size);
		if(acc0 == NULL)
		{
			printf("Nije moguca alokacija memorije za akumulator.\n");
			exit(1);
		}
		
		acc1 = (unsigned char *)malloc(acc_size);
		if(acc1 == NULL)
		{
			printf("Nije moguca alokacija memorije za akumulator.\n");
			exit(1);
		}

		copying(img, result_img, img_size);
		monochromating(img, gray_img, img_size);
		edging(gray_img, edge_img, width, height, gray_img_size);

		acc_init(acc0, rho);
		acc_init(acc1, rho);
		
		//Umesto funkcije hough(edge_img, acc, width, height, THRESHOLD);
		//transformacija se izvrsaava u IP_Core-u
		
		offset_bw = offset.to_default_time_units();
	
		write_hard(ADDR_RESET, 1); 
		while(!read_hard(ADDR_READY));
        	write_hard(ADDR_RESET, 0);
			
		write_hard(ADDR_IMG_WIDTH, width); 
		write_hard(ADDR_IMG_HEIGHT, height);
		write_hard(ADDR_THRESHOLD, THRESHOLD);
		write_hard(ADDR_RHO, rho);
		cout << "Zavrsena inicijalizacija od strane softvera" << endl;
		
		write_bram(VP_ADDR_ACC0_BRAM_L, acc0, acc_size);
		write_bram(VP_ADDR_ACC1_BRAM_L, acc1, acc_size);
		write_bram(VP_ADDR_IMG_BRAM_L, edge_img, gray_img_size);
		
		offset_bp = offset.to_default_time_units();
		
		write_hard(ADDR_START, 1);	
		while(read_hard(ADDR_READY));
        	write_hard(ADDR_START, 0);
		
		cout << "IP_Core aktivan" << endl;
		while(!read_hard(ADDR_READY));
		
		offset_ap = offset.to_default_time_units();
		
		//cout << "IP_Core zavrsio" << endl;
		
		read_bram(VP_ADDR_ACC0_BRAM_L, acc0, THETA2*rho);
		read_bram(VP_ADDR_ACC1_BRAM_L, acc1, THETA2*rho);
		
		offset_ar = offset.to_default_time_units();
		
		acc0_fp = fopen((const char*) acc0_file, "w");
		if (acc0_fp == NULL)
		{
			printf("Greska prilikom otvaranja fajla acc0!");
			return;
		}
		
		acc1_fp = fopen((const char*) acc1_file, "w");
		if (acc1_fp == NULL)
		{
			printf("Greska prilikom otvaranja fajla acc1!");
			return;
		}
		
		img_fp = fopen((const char*) img_file, "w");
		if (img_fp == NULL)
		{
			printf("Greska prilikom otvaranja fajla acc1!");
			return;
		}
		
		dim_fp = fopen((const char*) dim_file, "w");
		if (dim_fp == NULL)
		{
			printf("Greska prilikom otvaranja fajla acc1!");
			return;
		}
		
		for (j = 0; j < THETA2*rho; j++)
		{
			fprintf(acc0_fp, "%d", (int) acc0[j]);
			fprintf(acc1_fp, "%d", (int) acc1[j]);
			if (j != THETA2*rho - 1)
			{
				fprintf(acc0_fp, "\n");
				fprintf(acc1_fp, "\n");
			}
		}
		
		for (j = 0; j < height*width; j++)
		{
			fprintf(img_fp, "%d", (int) edge_img[j]);
			if (j != height*width - 1)
			{
				fprintf(img_fp, "\n");
			}
		}
		
		fprintf(dim_fp, "%d\n", rho);
		
		fclose(acc0_fp);
		fclose(acc1_fp);
		fclose(img_fp);
		fclose(dim_fp);
		
		
		//cout << "Ocitan akumulator" << endl;
		
		// nastavak softverskog dela
		
		acc_max = max_acc(acc0, acc1, rho);
		
		cout << "maksimalna vrednost akumulatora: " << (int) acc_max << endl;

		draw_lines(result_img, acc0, acc1, rho, width, height, acc_max, draw_factor);

		result_image[11] = ((i/2) % 10) + 48;
		if (i%20 == 0) result_image[10]++;
		stbi_write_jpg((const char*) result_image, width, height, channels, result_img, 100);
		
		offset_value_new = offset.to_default_time_units();
		offset_value = offset_value_new - offset_value_old;
	    	offset_value_old = offset_value_new;
	    	fps = 1000000000.0 / offset_value;
		
		fprintf(frame_fp, "Fotografija: %s\n", data_string[i+2]);
		fprintf(frame_fp, "Njene dimenzije: %dx%d\n", width, height);
		//fprintf(frame_fp, "Broj citanja iz deljlene memorije = %d\n", read_count);
		//fprintf(frame_fp, "Broj upisa u deljlenu memoriju = %d\n", write_count);
		fprintf(frame_fp, "Inicijalizacija i slanje podataka %lfus\n", (offset_bp-offset_bw)/1000.0);
		fprintf(frame_fp, "Obrada %lfus\n", (offset_ap-offset_bp)/1000.0);
		fprintf(frame_fp, "Citanje podataka %lfus\n", (offset_ar-offset_ap)/1000.0);
		fprintf(frame_fp, "Simulaciono vreme potrebno za transformaciju = %lfus\n", offset_value/1000.0);
		fprintf(frame_fp, "fps = %lf\n", fps);                 ;
		fprintf(frame_fp, "-----------------------------------\n\n");

		stbi_image_free(img);
		free(result_img);
		free(gray_img);
		free(edge_img);
		free(acc0);
		free(acc1);
	}
	
	fclose(frame_fp);
}

void Cpu::monochromating(unsigned char *img, unsigned char *gray_img, size_t img_size)
{
    unsigned char *p;
    unsigned char *pg;

    for(p = img, pg = gray_img; p != img + img_size; p += 3, pg++)
    {
        *pg = (uint8_t) ((*p + *(p + 1) + *(p + 2))/3.0);
    }
}

void Cpu::copying(unsigned char *img, unsigned char *same_img, size_t img_size)
{
    unsigned char *p;
    unsigned char *pg;

    for(p = img, pg = same_img; p != img + img_size; p++, pg++)
    {
        *pg = *p;
    }
}

void Cpu::edging(unsigned char *img, unsigned char *edge_img, int width, int height, size_t img_size)
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

void Cpu::acc_init(unsigned char * acc, int rho)
{
    int16_t i, j;

    for (i = 0; i < THETA2; i++)
    {
        for (j = 0; j < rho; j++)
        {
            *(acc + i * rho + j) = 0;
        }
    }
}

int16_t Cpu::max_acc(unsigned char *acc0, unsigned char *acc1, int rho)
{
    int16_t i, j;
    int16_t acc_max = 0;
    
    for (i = 0; i < THETA2; i++)
    {
        for (j = 0; j < rho; j++)
        {
            if(*(acc0 + i * rho + j) > acc_max)
            {
            	acc_max = *(acc0 + i * rho + j);
            }
            
            if(*(acc1 + i * rho + j) > acc_max)
            {
            	acc_max = *(acc1 + i * rho + j);
            }
        }
    }

    return acc_max;
}

void Cpu::draw_lines(unsigned char *img, unsigned char *acc0, unsigned char *acc1, int rho, int width, int height, int16_t acc_max, dw_t draw_factor)
{
    int16_t acc_tres;
    int16_t i, j, j_tmp;
    trig_t a, b;
    point_t x0, x1, y0, y1;
    slope_t slope;
    intercept_t intercept;
    int16_t x, y;
    int16_t y_new, y_old = 0;
    //cout << "Ispis linija moze da pocne, vrednosti su: " << endl;

    acc_tres = draw_factor*acc_max;

    for (i = 0; i < rho; i++)
    {
        for (j= 0; j < THETA2; j++)
        {
            j_tmp = 2 * j;
            
            if (*(acc0 + j * rho + i) >= acc_tres) 
            {
            	//cout << "theta = " << theta << ", rho = " << rho << endl;
               if (j_tmp >= THETA)
                {
                    a = cos((j_tmp-THETA)*M_PI/180);
                    b = sin((j_tmp-THETA)*M_PI/180);
                    x0 = - b * i;
                    y0 = a * i;
                }
                else
                {
                    a = cos(j_tmp*M_PI/180);
                    b = sin(j_tmp*M_PI/180);
                    x0 = b * i;
                    y0 = - a * i;
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
                        *(img + 3*(y*width + i)) = 255;
                        *(img + 3*(y*width + i) + 1) = 0;
                        *(img + 3*(y*width + i) + 2) = 0; 
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
            
            j_tmp++;
            
            if (*(acc1 + j * rho + i) >= acc_tres) 
            {
            	//cout << "theta = " << theta << ", rho = " << rho << endl;
               if (j_tmp >= THETA)
                {
                    a = cos((j_tmp-THETA)*M_PI/180);
                    b = sin((j_tmp-THETA)*M_PI/180);
                    x0 = - b * i;
                    y0 = a * i;
                }
                else
                {
                    a = cos(j_tmp*M_PI/180);
                    b = sin(j_tmp*M_PI/180);
                    x0 = b * i;
                    y0 = - a * i;
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
                        *(img + 3*(y*width + i)) = 255;
                        *(img + 3*(y*width + i) + 1) = 0;
                        *(img + 3*(y*width + i) + 2) = 0; 
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

void Cpu::write_bram(sc_dt::uint64 addr, unsigned char * val, int length)
{
	offset += sc_core::sc_time(10*DELAY , sc_core::SC_NS);

	pl_t pl;
	unsigned char buf[4] = {0, 0, 0, 0};

	for(int i = 0; i < length; i++)
	{
		buf[3] = val[i];
		pl.set_address(addr + i);
		pl.set_data_ptr(buf);
		pl.set_command(tlm::TLM_WRITE_COMMAND);
		pl.set_data_length(BUS_WIDTH);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		interconnect_socket->b_transport(pl, offset);
		write_count += 1;
		offset += sc_core::sc_time(12*DELAY, sc_core::SC_NS);
	}
}

void Cpu::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{
	offset += sc_core::sc_time(10*DELAY , sc_core::SC_NS);		
	
	pl_t pl;	
	unsigned char buf[4];
    
	for(int i = 0; i < length; i++)
	{
	    	pl.set_address(addr + i);
	    	pl.set_data_length(BUS_WIDTH); 
		pl.set_data_ptr(buf);
		pl.set_command(tlm::TLM_READ_COMMAND);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		interconnect_socket->b_transport(pl, offset);
		read_count += 1;
		val[i] = buf[3]; 	
		offset += sc_core::sc_time(10*DELAY, sc_core::SC_NS);
	}
}

int Cpu::read_hard(sc_dt::uint64 addr)
{
    pl_t pl;
    unsigned char buf[8];
    pl.set_address(VP_ADDR_IP_HARD_L + addr);
    pl.set_data_length(1);
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_READ_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    interconnect_socket->b_transport(pl,offset);
    return toInt(buf);
}

void Cpu::write_hard(sc_dt::uint64 addr, int val)
{
    pl_t pl;
    unsigned char buf[4];
    toUchar4(buf,val); 	
    pl.set_address(VP_ADDR_IP_HARD_L + addr);
    pl.set_data_length(1);
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_WRITE_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    interconnect_socket->b_transport(pl,offset);
}
