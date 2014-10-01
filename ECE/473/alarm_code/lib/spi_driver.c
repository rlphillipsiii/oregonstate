#include <avr/io.h>
#include "macros.h"

void spi_init()
{
	DDRB |= 0x07;  //set output mode for SS, MOSI, SCLK
	SPCR = (1 << SPE) | (1 << MSTR); //set master mode, clk low on idle, leading edge sample
	SPSR = (1 << SPI2X);  //set choos
}

void spi_wait()
{
	while (!ISSET(SPSR, SPIF));
}

char spi_send(char msg)
{
	SPDR = msg; //set SPDR to start the transmission

	while (!ISSET(SPSR, SPIF)); // wait until transmission is complete
	
	return SPDR; // return the byte that was received
}