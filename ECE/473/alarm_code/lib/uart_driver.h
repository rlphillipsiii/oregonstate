
//UART Functions header file 
//Roger Traylor 11.l6.11
//For controlling the UART and sending debug data to a terminal
//as an aid in debugging.
#ifndef UART_DRIVER
#define UART_DRIVER

void uart_putc(char data);
void uart_puts(char *str);
void uart_puts_p(const char *str);
void uart_init();
char uart_getc(void);

void enable_rx_interrupts();

#endif
