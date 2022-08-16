/*
 * spi.c
 *
 *  Created on: 2022. 8. 13.
 *      Author: taeju
 *
 *
 *	LaunchPadXL TMS320F28069M
 *	SPI Pin 
 *
 *	SPI A
 *	J1.7	GPIO18	CLK
 *	J2.19	GPIO19	/CS	
 *	J2.15	GPIO16	SI (Slave In)
 *	J2.14	GPIO17	SO (Slave Out)
 *
 * Manual 775 page 
 *
 *
 *
 *
 *
 */
#include "define.h"


Uint16 spi_sdata=0;  // send data
Uint16 spi_rdata=0;  // received data



static void error(void)
{
    __asm("     ESTOP0");						// Test failed!! Stop!
    for (;;);
}

//
// spi_init - 
//
static void spi_register_init(void)
{    
    SpiaRegs.SPICCR.all =0x000F;  // Reset on, rising edge, 16-bit char bits  
    
    //
    // Enable master mode, normal phase, enable talk, and SPI int disabled.
    //
    SpiaRegs.SPICTL.all =0x0006;
    
    SpiaRegs.SPIBRR =0x007F;									
    SpiaRegs.SPICCR.all =0x009F;   // Relinquish SPI from Reset   
    SpiaRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission
}


//
// spi_xmit - 
//
static void spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
}    

//
// spi_fifo_init - 
//
static void spi_fifo_init(void)										
{
    //
    // Initialize SPI FIFO registers
    //
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x2044;
    SpiaRegs.SPIFFCT.all=0x0;
}  



void spi(void)
{
	spi_xmit(spi_sdata); // transmit data

 

	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1)	{

	}

	spi_rdata = SpiaRegs.SPIRXBUF;
	if(spi_sdata != spi_rdata)	{
		error();
	}
	spi_sdata++;
}

void spi_Init(void)
{
	spi_fifo_init();
	spi_register_init();
}

