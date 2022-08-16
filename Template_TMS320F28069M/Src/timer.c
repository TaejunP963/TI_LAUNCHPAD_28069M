/*
 * timer.c
 *
 *  Created on: 2022. 8. 13.
 *      Author: taeju
 */
#include "define.h"

void timer_Init(void)
{
    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;  // This is needed to write to EALLOW protected registers

#if (CPU_TIMER_0 == 1)
    PieVectTable.TINT0 = &cpu_timer0_isr;
#endif

#if (CPU_TIMER_1 == 1)
    PieVectTable.TINT1 = &cpu_timer1_isr;
#endif

#if (CPU_TIMER_2 == 1)
    PieVectTable.TINT2 = &cpu_timer2_isr;
#endif
    EDIS;    // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize the Device Peripheral. This function can be
    //         found in F2806x_CpuTimers.c
    //
    InitCpuTimers();   // For this example, only initialize the Cpu Timers

    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 80MHz CPU Freq, 1 second Period (in uSeconds)
    //
#if (CPU_TIMER_0 == 1)    
    ConfigCpuTimer(&CpuTimer0, 80, 1000);
#endif

#if (CPU_TIMER_1 == 1)
    ConfigCpuTimer(&CpuTimer1, 80, 100000);
#endif

#if (CPU_TIMER_2 == 1)
    ConfigCpuTimer(&CpuTimer2, 80, 100000);
#endif

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the below
    // settings must also be updated.
    //

    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer0Regs.TCR.all = 0x4000;

    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer1Regs.TCR.all = 0x4000;

    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer2Regs.TCR.all = 0x4000;

    //
    // Step 5. User specific code, enable interrupts
    //

    //
    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2
    //
    IER |= M_INT1;
    IER |= M_INT13;
    IER |= M_INT14;

    //
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}
