// mega48.c
// Robert Phillips III
// 10.3.05
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <uart_driver.h>
#include <thermo_driver.h>

extern volatile uint8_t lm73_rd_buf[2];

int main()
{
	uart_init();
	init_thermo_sensors();
	
	sei();

	struct temperature temp_data;
	prepare_local_read(1);

	while (1) {
		local_thermo_read(1);

		uint8_t ms = (lm73_rd_buf[0] << 1);
		uint8_t ls = (lm73_rd_buf[1] >> 7);

		uart_putc(ms | ls);

		_delay_ms(225);
		_delay_ms(225);
		
		_delay_ms(225);
		_delay_ms(225);
	}
}  //main

