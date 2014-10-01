#include <math.h>
#include <avr/io.h>
#include "macros.h"

#define ADC_MUX_MASK 0b00011111
#define SIZE 1050

static uint8_t lookup[SIZE];

static void init_lookup_table()
{
	uint16_t i;
	for (i = 0; i < SIZE; i++)
		lookup[i] = (uint8_t) round((-49.0f/195.0f)*i+(3560.0f/13.0f));
		//lookup[i] = (uint8_t) round(.7f*i+290);
}

void enable_adc()
{
	CLRBIT(DDRF, PF7);
	CLRBIT(PORTF, PF7);

	ADMUX  = (1 << REFS0) | (PF7 & ADC_MUX_MASK);    //single-ended port F bit 7, right adjusted for 10 bits
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);    //ADC enabled, don't start yet, single shot mode	

	init_lookup_table();
}

void start_adc()
{
	SETBIT(ADCSRA, ADSC);
}

uint16_t poll_adc()
{
	start_adc();

	while (!ISSET(ADCSRA, ADIF));
	SETBIT(ADCSRA, ADIF);

	return ADC;
}

uint8_t is_adc_running()
{
	if (!ISSET(ADCSRA, ADIF))
		return 1;

	return 0;
}

uint16_t get_adc()
{
	return ADC;
}

uint8_t get_adc_adjusted(uint16_t raw)
{
   if (raw > 1000)
		return 20;
	
	if (raw < 76)
		return 255;

	if (lookup[raw] > 50)
		return lookup[raw]-50;
	
	return lookup[raw];
}
