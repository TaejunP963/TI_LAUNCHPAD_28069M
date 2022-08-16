

/**
 * main.c
 * LAUNCHPADXL TMS320F28069M
 * LED  : D1 - 3.3V
 *      : D9 - GPIO34
 *      : D10 - GPIO39
 *
 * S1 BOOT Mode
 * TRST TDO GPIO34
 *  1   X   X       Emulation Boot
 *  0   0   0       Parallel I/O
 *  0   0   1       SCI
 *  0   1   0       Wait
 *  0   1   1       GetMode
 *
 *  CH_SEL  MUX_SEL
 *  JP6     JP7
 *   0       0        UART Disable ; GPIO28&29 — J1.3 & J1.4 ; GPIO15&58 — J7.3 & J7.4 .
 *   0       1        U11 & U15 Disable ; GPIO15&58 — J7.3 & J7.4 ; GPIO28&29 — UART .
 *   1       0        U16 Disable ; GPIO28&29 — Fault & Octw ; GPIO15&58 — UART.
 *   1       1        U16 Disable ; GPIO28&29 — Fault & Octw ; GPIO15&58 — UART.
 *
 *   JP 1   Enable 3.3 V from USB (disables isolation)
 *   JP 2   Enable GND from USB (disables isolation)
 *   JP 3   Enable 5 V switcher (powered off 3.3 V supply of target device)
 *   JP 4   Connects target MCU 3.3 V to second set of BoosterPack headers
 *   JP 5   Connects target MCU 5 V to second set of BoosterPack headers
 *
 *
 *
 *
 *
 */
#include "stdbool.h"
#include "define.h"


void main(void)
{
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2806x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initalize GPIO:
    // This example function is found in the F2806x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    // InitGpio();  // Skipped for this example

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags are cleared.
    // This function is found in the F2806x_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2806x_DefaultIsr.c.
    // This function is found in F2806x_PieVect.c.
    //
    InitPieVectTable();

	timer_Init();
	spi_Init();


    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

	for(;;)
    {
		spi();



    }

}

