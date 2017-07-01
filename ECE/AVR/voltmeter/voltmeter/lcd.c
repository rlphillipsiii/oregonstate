/*
 * lcd.c
 *
 * Created: 11/21/2012 11:09:01 PM
 *  Author: Robert Phillips III
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void strobe_lcd(){
	PORTF = 0x08;
	PORTF = 0x00;
}

void clear_display(){
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x01;
	_delay_us(2);
	strobe_lcd();
	_delay_us(1900);
}

void cursor_home(){
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x02;
	_delay_us(2);
	strobe_lcd();
	_delay_us(1900);
}

void home_line2(){
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0xC0;
	_delay_us(2);
	strobe_lcd();
	_delay_us(1900);
}

void char2lcd(char a_char){
	SPDR = 0x01;
	_delay_us(2);
	SPDR = a_char;
	_delay_us(2);
	strobe_lcd();
	_delay_us(160);
}

void string2lcd(char *lcd_str){
	int count;
	for (count=0; count<=(strlen(lcd_str)-1); count++){
		SPDR = 0x01;
		_delay_us(2);
		SPDR = lcd_str[count];
		_delay_us(2);
		strobe_lcd();
		_delay_us(160);
	}
}

void spi_init(){
	DDRF=0x08;
	PORTB=0x00;
	DDRB=0x07;
	
	SPCR=0x50;
	SPSR=0x01;
}

void lcd_init(){
	_delay_ms(15);
	
	int i;
	for(i=0; i<=2; i++){
		SPDR = 0x00;
		_delay_us(2);
		SPDR = 0x30;
		_delay_us(2);
		strobe_lcd();
		_delay_us(6100);
	}
	
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x38;
	_delay_us(2);
	strobe_lcd();
	_delay_us(4100);
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x08;
	_delay_us(2);
	strobe_lcd();
	_delay_us(4100);
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x01;
	_delay_us(2);
	strobe_lcd();
	_delay_us(4100);
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x06;
	_delay_us(2);
	strobe_lcd();
	_delay_us(4100);
	SPDR = 0x00;
	_delay_us(2);
	SPDR = 0x0E;
	_delay_us(2);
	strobe_lcd();
	_delay_us(4100);
}