#ifndef LCD_DRIVER
#define LCD_DRIVER

/******************************************************
 * Sets all 32 characters on the screen to spaces.
 *****************************************************/
void clear_display();

/******************************************************
 * Sets the next character location to be the top left
 * character.
 *****************************************************/
void cursor_home();

/******************************************************
 * Sets the next character location to be the bottom
 * left character.
 *****************************************************/
void home_line2();

/******************************************************
 * Fills a single row on the LCD screen with spaces.
 *****************************************************/
void fill_spaces();

/******************************************************
 * Sends a single character to the LCD screen.
 *****************************************************/
void char2lcd(char a_char);

/******************************************************
 * Sends a whole string to the LCD screen.
 *****************************************************/
void string2lcd(char *lcd_str);

/******************************************************
 * Enables the LCD screen for use.
 *****************************************************/
void enable_lcd();

#endif
