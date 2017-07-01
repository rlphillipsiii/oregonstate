/*
 * voltmeter.c
 *
 * Created: 11/21/2012 10:54:27 PM
 *  Author: Robert Phillips III
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lcd.h"

#define POS_ADC 0
#define NEG_ADC 1

#define ADC_MUX_MASK 0b00011111
#define ISSET(byte, bit) byte & (1 << bit)

float x1[] = {4.28, 3.82, 3.33, 2.45, 1.52, 0.52};
float y1[] = {14.29, 12.71, 11.11, 8.18, 5.08, 1.76};
		
float x2[] = {0.81, 1.69, 2.68, 3.31, 4.86};
float y2[] = {-14.21, -11.1, -8.09, -6.11, -1.72};
	
float x1_err[] = {996, 724, 534, 224, 117};
float y1_err[] = {2.45, 1.38, 1.09, 0.45, 0.22};
		
float x2_err[] = {155, 242, 500, 707, 1009};
float y2_err[] = {0.19, 0.13, 0.35, 0.46, 0.12};
uint16_t res;

float lagrange(float x, int size, float xvals[], float yvals[]);

int byte_to_int(unsigned char byte){
	int i;
	uint8_t result = 0;
	
	for (i = 0; i < 8; i++) {
		if (ISSET(byte, i))
			result += pow(2, i);
	}	
	
	return result;
}

void init(){	
	/* enable ADC */
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	
	spi_init();
	lcd_init();
	
	clear_display();
}

void set_adc(int channel){
	PORTF &= ~(1 << channel);
	ADMUX = (1 << REFS0) | (channel & ADC_MUX_MASK);
	
	ADCSRA |= (1 << ADSC);
	while (ISSET(ADCSRA, ADSC));
}

uint16_t do_adc(int channel){
	set_adc(channel);
	
	ADCSRA |= (1 << ADSC);
	while (ISSET(ADCSRA, ADSC));
	
	unsigned char low = ADCL;
	unsigned char high = ADCH;

	return (high << 8) | low;
}	

float interpret_adc(int channel){
	uint16_t result = do_adc(channel);
	
	res = result;
	return 4.98*(result/1024.0);
}

float lagrange_sub(float x, int index, float xvals[], int xsize, float y){
	float val = 1;
	float sub = xvals[index];
	
	int i;
	for (i = 0; i < xsize; i++) {
		float tmp = xvals[i];
		
		if (i != index)
			val *= ((x-tmp)/(sub-tmp));
	}
	
	return val*y;
}

float lagrange(float x, int size, float xvals[], float yvals[]){
	float ans = 0;
	
	int i;
	for (i = 0; i < size; i++)
		ans += lagrange_sub(x, i, xvals, size, yvals[i]);

	return ans;
}

double get_voltage(int channel){
	float result = interpret_adc(channel);
		
	if (channel == POS_ADC)
		return lagrange(result, 6, x1, y1) - lagrange((float) res, 5, x1_err, y1_err);
	else
		return lagrange(result, 5, x2, y2) - lagrange((float) res, 5, x2_err, y2_err) + 0.15;
}

int main(void)
{
	init();
	
	float pos = 0, neg = 0, cur;
	char text[17];	
	int index;
	
    while(1){
		memset(text, 32, sizeof(text));
		
		cur = get_voltage(POS_ADC);
		
		if (fabs(pos - cur) > .1){
			cursor_home();
			index = sprintf(text, "+%2.2fV", cur);
			text[index] = 32;
			text[16] = 0;
			string2lcd(text);
			
			pos = cur;
		}
				
		
		cur = get_voltage(NEG_ADC);
		
		if (fabs(neg - cur) > .1){
			home_line2();
			index = sprintf(text, "%2.2fV", cur);
			text[index] = 32;
			text[16] = 0;
			string2lcd(text);
			
			neg = cur;
		}	
		
		_delay_ms(100);
	}
}