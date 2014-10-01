#include <avr/io.h>
#include "spi_driver.h"
#include "macros.h"

#define STROBE_BIT  PC0
#define STROBE_PORT PORTC

void enable_graph()
{
	DDRC |= 0x01; // set PD0 to an output
	
	CLRBIT(STROBE_PORT, STROBE_BIT); // clear PD0
	spi_init(); // initialize the spi interface
}

char update_graph(char value)
{
	char result = spi_send(value); // send the given value via spi
	strobel(STROBE_PORT, STROBE_BIT); // set PD0 and then clear it

	return result; // return the result read from spi
}
