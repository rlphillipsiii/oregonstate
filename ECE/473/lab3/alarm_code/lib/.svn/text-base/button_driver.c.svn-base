#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>

#define BYTE 8

void enable_buttons()
{
	// disable the segments without enabling the tristate buffer
	PORTB &= ~(1 << PB4);
	PORTB |= (1 << PB6) | (1 << PB5);

	DDRA = 0; // change DDRA to accept input
	PORTA = 0xFF; // enable internal pullups on PORTA

	_delay_us(10);
	PORTB |= (1 << PB4) | (1 << PB5) | (1 << PB6); // enable the tristate buffer
}

uint8_t debounce_switch(uint16_t *state, uint8_t bit)
{
    *state = (*state << 1) | (!bit_is_clear(PINA, bit)) | 0xE000;
	
    if (*state == 0xF000)
        return 1;

    return 0;
}

char poll_buttons(char reg)
{
	char buttons = 0;

	static uint16_t state[] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	uint8_t i;
	for (i = 0; i < BYTE; i++) {
		if (debounce_switch(&state[i], i))
			buttons |= (1 << i);
	}

	return buttons;
}
