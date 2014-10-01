#include <avr/io.h>
#include "spi_driver.h"
#include "macros.h"

void enable_graph()
{
	DDRD |= 0x01; // set PD0 to an output
	
	CLRBIT(PORTD, PD0); // clear PD0
	spi_init(); // initialize the spi interface
}

char update_graph(char value)
{
	char result = spi_send(value); // send the given value via spi
	strobel(PORTD, PD0); // set PD0 and then clear it

	return result; // return the result read from spi
}
