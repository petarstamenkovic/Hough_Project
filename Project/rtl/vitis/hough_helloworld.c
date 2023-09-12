/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "xtime_l.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "tb_img.h"

#define START_REG_OFFSET 0
#define RESET_REG_OFFSET 4
#define WIDTH_REG_OFFSET 8
#define HEIGHT_REG_OFFSET 12
#define RHO_REG_OFFSET 16
#define TRESHOLD_REG_OFFSET 20
#define READY_REG_OFFSET 24

#define TRESHOLD_C 140
#define HALF_THETA_C 135

#define ADDR_FACTOR 4

int main()
{
	#ifdef TB_IMG
    	int val0, val1;
	#endif

	float duration, d1, d2, d3, frame_rate;
	XTime p0_time, p1_time, p2_time, p3_time;
	XTime *p_p0_time = &p0_time;
	XTime *p_p1_time = &p1_time;
	XTime *p_p2_time = &p2_time;
	XTime *p_p3_time = &p3_time;

    init_platform();
    Xil_DCacheDisable();
    Xil_ICacheDisable();
	
    // Enabling burst-mode of AXI BRAM Controllers
    Xil_SetTlbAttributes(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR, 0x15de6);
	Xil_SetTlbAttributes(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR, 0x15de6);
	Xil_SetTlbAttributes(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR, 0x15de6);

	printf("------------------------------------\n");
	printf("--- HOUGH TRANSFORMATION IP CORE ---\n");
	printf("------------------------------------\n\n");

    // Time at the beginning
    XTime_GetTime(p_p0_time);

    // Initialization of the system
    Xil_Out8(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + START_REG_OFFSET, 0);
    Xil_Out8(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + RESET_REG_OFFSET, 1);
    while(!Xil_In32(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + READY_REG_OFFSET));
    Xil_Out8(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + RESET_REG_OFFSET, 0);

    // Sending parameters to Hough-core
    Xil_Out16(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + WIDTH_REG_OFFSET, WIDTH_C);
    Xil_Out16(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + HEIGHT_REG_OFFSET, HEIGHT_C);
    Xil_Out16(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + RHO_REG_OFFSET, RHO_C);
    Xil_Out8(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + TRESHOLD_REG_OFFSET, TRESHOLD_C);

    // Sending zeros to Hough-core's accumulators
    memcpy((void*)XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR, acc0, sizeof(acc0));
	memcpy((void*)XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR, acc1, sizeof(acc1));

    // Sending image to Hough-core
	memcpy((void*)XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR, case_img, sizeof(case_img));

	// Time after initialization and sending data to the Hough-core
    XTime_GetTime(p_p1_time);

    // Starting processing
    Xil_Out8(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + START_REG_OFFSET, 1);
    while(Xil_In32(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + READY_REG_OFFSET));
    Xil_Out8(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + START_REG_OFFSET, 0);

    // Waiting for the core to finish processing
    while(!Xil_In32(XPAR_HOUGH_IP_0_S00_AXI_BASEADDR + READY_REG_OFFSET));
    //printf("DONE!\n");

    // Time after processing
    XTime_GetTime(p_p2_time);

    // Reading Hough-core's accumulators afters processing
    memcpy(acc0, (void*)XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR, sizeof(acc0));
    memcpy(acc1, (void*)XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR, sizeof(acc1));

    // Time after reading
    XTime_GetTime(p_p3_time);

    // Duration of the initialisation and sending data to the Hough-core
    d1 = 1.0 * ((int) p1_time - (int) p0_time) / (COUNTS_PER_SECOND / 1000000);

    // Duration of the core's processing
    d2 = 1.0 * ((int) p2_time - (int) p1_time) / (COUNTS_PER_SECOND / 1000000);

    // Duration of the reading data from the Hough-core
    d3 = 1.0 * ((int) p3_time - (int) p2_time) / (COUNTS_PER_SECOND / 1000000);

    // Total duration
    duration = d1 + d2 + d3;

    // Calculation of the frame_rate
    frame_rate = 1000000.0 / duration;

    printf("Duration is %.2f[us].\n", duration);
    printf("Frame rate is %.2f[fps].\n", frame_rate);

	#ifdef TB_IMG
		printf("Result checking...\n");
		val0 = acc0[452];
		val1 = acc1[1014];
		printf("acc0[%d] = %d\n", (30 * RHO_C + 2), val0);
		printf("acc1[%d] = %d\n", (67 * RHO_C + 9), val1);

		val0 = acc0[1578];
		val1 = acc1[330];
		printf("acc0[%d] = %d\n", (105 * RHO_C + 3), val0);
		printf("acc1[%d] = %d\n", (22 * RHO_C + 0), val1);
		printf("... DONE!\n");
	#endif

	printf("\n--------------- EXIT ---------------\n");
	printf("------------------------------------\n\n\n\n\n");

    cleanup_platform();
    return 0;
}
