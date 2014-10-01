#define F_CPU 16000000UL //16Mhz clock

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "spi_driver.h"

#define NUM_LCD_CHARS 16 //number of chars that the LCD has in a line

void clear_display(){
	spi_send(0x00);
	spi_send(0x01);

	strobel(PORTF, PF3);
	_delay_ms(2);   //obligatory waiting for slow LCD (1.64mS)
}         

void cursor_home(){
	spi_send(0x00);
	spi_send(0x02);

	strobel(PORTF, PF3);
	_delay_ms(2);   //obligatory waiting for slow LCD (1.64mS)
}         
  
void home_line2(){
	spi_send(0x00);
	spi_send(0xC0);
	
	strobel(PORTF, PF3);
	_delay_ms(2);   //obligatory waiting for slow LCD (1.64mS)
}                           
 
void fill_spaces(){
	uint8_t i;
	for (i = 0; i < NUM_LCD_CHARS; i++){
		spi_send(0x01);
		spi_send(0x20);
	  
		strobel(PORTF, PF3);
		_delay_us(100);   //obligatory waiting for slow LCD (40uS)
	}
}  
   
void char2lcd(char a_char){
	spi_send(0x01);
	spi_send(a_char);
	
	strobel(PORTF, PF3);
	_delay_us(100);   //obligatory waiting for slow LCD (40uS)
}
  
  
void string2lcd(char *lcd_str){
	uint8_t i;
	for (i = 0; i < strlen(lcd_str); i++){
		spi_send(0x01);
		spi_send(lcd_str[i]);

		strobel(PORTF, PF3);
		_delay_us(100);   //obligatory waiting for slow LCD (40uS)
	}                  
} 

void enable_lcd(){
	uint8_t i;

	DDRF |= (1 << PF3);
	PORTF |= (1 << PF3);
	spi_init();

	_delay_ms(15);   
	for(i=0; i<=2; i++){ //do funky initalize sequence 3 times
		spi_send(0x00);
		spi_send(0x30);

		strobel(PORTF, PF3);
		_delay_ms(7);
	}

	spi_send(0x00);
	spi_send(0x38);

	strobel(PORTF, PF3);
	_delay_ms(5);   

	spi_send(0x00);
	spi_send(0x08);

	strobel(PORTF, PF3);
	_delay_ms(5);

	spi_send(0x00);
	spi_send(0x01);

	strobel(PORTF, PF3);
	_delay_ms(5);   

	spi_send(0x00);
	spi_send(0x06);

	strobel(PORTF, PF3);
	_delay_ms(5);

	spi_send(0x00);
	spi_send(0x0C);
	
	strobel(PORTF, PF3);
	_delay_ms(5);
} 
