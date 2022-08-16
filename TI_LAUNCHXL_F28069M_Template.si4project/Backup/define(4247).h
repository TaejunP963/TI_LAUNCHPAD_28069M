/*
 * define.h
 *
 *  Created on: 2022. 8. 13.
 *      Author: taeju
 */

#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "DSP28x_Project.h"
#include "math.h"

/*********************************************
		spi.c
**********************************************/
void spi(void);
void spi_Init(void);




/*********************************************
		Timer.c
**********************************************/



void timer_Init(void);
/*********************************************
		interrupt.c
**********************************************/
#define SINE_WAVE       1

#define CPU_TIMER_0		1
#define CPU_TIMER_1		1
#define CPU_TIMER_2		1

__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);


#endif /* __DEFINE_H__ */
