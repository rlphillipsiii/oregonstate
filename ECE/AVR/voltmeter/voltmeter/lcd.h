/*
 * lcd.h
 *
 * Created: 11/21/2012 11:09:23 PM
 *  Author: Robert
 */ 


#ifndef LCD_H_
#define LCD_H_

void home_line2();
void cursor_home();
void strobe_lcd();
void clear_display();
void char2lcd(char a_char);
void string2lcd(char *lcd_str);
void spi_init();
void lcd_init();

#endif /* LCD_H_ */