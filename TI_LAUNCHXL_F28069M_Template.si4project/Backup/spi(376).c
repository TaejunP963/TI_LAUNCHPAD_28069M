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
 *	Clock polarity(CPOL)	Clock phase(CPHA)
 *		0						0				CLK : Low, data read : 1 Edge(rising edge)
 *		0						1				CLK : Low, data read : 2 Edge(falling edge)
 *		1						0				CLK : High, data read : 1 Edge(rising edge)
 *		1						1				CLK : High, data read : 2 Edge(falling edge)
 */
#include "define.h"
#include "AT45DB041D.h"



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
#if (AT45DB041_SPI_INIT == 1)
	// SPI Configuration Control Register(SPICCR)
	SpiaRegs.SPICCR.bit.SPISWRESET = 1; // spi software reset
	SpiaRegs.SPICCR.bit.SPILBK = 0; //loopback mode disable
	SpiaRegs.SPICCR.bit.CLKPOLARITY = 1; 
	SpiaRegs.SPICCR.bit.SPICHAR = 8-1; // data length - 1

	// SPI Operation Control(SPICTL)
	SpiaRegs.SPICTL.bit.CLK_PHASE = 1; //
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; // SPI is Configured as a SLAVE and MASTER 
	SpiaRegs.SPICTL.bit.TALK = 1; // SPI Transmit Enable
	SpiaRegs.SPICTL.bit.SPIINTENA = 0; // SPI Interrupt Enable

	// SPI Baud Rate Register
	SpiaRegs.SPIBRR = 0x007F;
	// 0, 1, 2 SPI Baud Rate = LSPCLK / 4
	// 3 Tto 127 SPI Baud Rate = LSPCLK / (SPIBRR + 1)

	// 0x0003, LSPCLK/4
	// 0x0004, LSPCLK/5
	// 0x007E, LSPCLK/127
	// 0x007F, LSPCLK/128

	//SpiaRegs.SPIPRI.bit.FREE = 1;
	
#else
    SpiaRegs.SPICCR.all =0x000F;  // Reset on, rising edge, 16-bit char bits  
    
    //
    // Enable master mode, normal phase, enable talk, and SPI int disabled.
    //
    SpiaRegs.SPICTL.all =0x0006;
    
    SpiaRegs.SPIBRR =0x007F;									
    SpiaRegs.SPICCR.all =0x009F;   // Relinquish SPI from Reset   
    SpiaRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission
#endif
}



//
// spi_fifo_init - 
//
static void spi_fifo_init(void)										
{
#if (AT45DB041_SPI_INIT == 1)

#else
    //
    // Initialize SPI FIFO registers
    //
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x2044;
#endif
    SpiaRegs.SPIFFCT.all=0x0;
}  

static void spi_gpio_init(void)
{
    EALLOW;
    
    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;  // Enable pull-up on GPIO3 (SPISOMIA)
    //GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;  // Enable pull-up on GPIO5 (SPISIMOA)

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;  // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;  // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;  // Enable pull-up on GPIO19 (SPISTEA)

    //GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0; // Enable pull-up on GPIO54 (SPISIMOA)
    //GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0; // Enable pull-up on GPIO55 (SPISOMIA)
    //GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0; // Enable pull-up on GPIO56 (SPICLKA)
    //GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0; // Enable pull-up on GPIO57 (SPISTEA)

    //
    // Set qualification for selected pins to asynch only
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //

    //GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    //GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO55 (SPISOMIA)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

    //
    // Configure SPI-A pins using GPIO regs
    // This specifies which of the possible GPIO pins will be SPI functional
    // pins.
    // Comment out other unwanted lines.
    //

    //GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 2;  // Configure GPIO3 as SPISOMIA
    //GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;  // Configure GPIO5 as SPISIMOA

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    //GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SPISIMOA
    //GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SPISOMIA
    //GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1; // Configure GPIO56 as SPICLKA
    //GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1; // Configure GPIO57 as SPISTEA

    EDIS;


}

//
// spi_xmit - 
//
void spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
}    

Uint16 spi_reci(void)
{
	return SpiaRegs.SPIRXBUF;
}

void spi(void)
{
	spi_xmit(0x9F); // transmit data

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
	spi_gpio_init();

	spi_fifo_init();
	spi_register_init();
}
