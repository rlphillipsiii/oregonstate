#include <util/delay.h>

#include "twi_master.h"
#include "lm73_functions.h"
#include "thermo_driver.h"
#include "macros.h"

volatile uint8_t lm73_wr_buf[2];
volatile uint8_t lm73_rd_buf[2];

void init_thermo_sensors()
{
	init_twi();
}

void prepare_local_read(uint8_t wait)
{
	lm73_wr_buf[0] = LM73_PTR_TEMP;//load lm73_wr_buf[0] with temperature pointer address                                                      
    twi_start_wr(LM73_ADDRESS, lm73_wr_buf, 1); //start the TWI write process

	if (wait)
		_delay_ms(2);
}

void local_thermo_read(uint8_t wait)
{
	twi_start_rd(LM73_ADDRESS, lm73_rd_buf, 2); //read temperature data from LM73 (2 bytes)
	
	if (wait)
		_delay_ms(2);
}

#ifdef MEGA128
void decode_temp(struct temperature *temp)
{
	uint16_t raw = temp->raw;
	
	temp->positive = TRUE;
	if (raw & 0x8000) {
		temp->positive = FALSE;
		
		raw = (~raw)+1;
	}
	
	uint16_t whole = raw >> 7;

	temp->celsius = (uint8_t) whole;
	temp->fahrenheit = (uint8_t) temp->celsius*1.8+32;
}
#endif

void get_local_temp(struct temperature *temp)
{
	temp->raw = (lm73_rd_buf[0] << 8) | lm73_rd_buf[1];
	
#ifdef MEGA128
	decode_temp(temp);
#endif
}
