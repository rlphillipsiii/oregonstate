#ifndef LCD_DRIVER
#define LCD_DRIVER

void clear_display();
void cursor_home();
void home_line2();      
void fill_spaces();
void char2lcd(char a_char);
void string2lcd(char *lcd_str);
void enable_lcd();

#endif
