#ifndef CLOCK
#define CLOCK

#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect);

void init_clock_timer();

void set_time(uint8_t hr, uint8_t min);
void set_alarm_state(uint8_t state);

void change_hour(uint8_t delta);
void change_minute(uint8_t delta);
void change_alarm_hour(uint8_t delta);
void change_alarm_minute(uint8_t delta);

uint8_t is_alarm_time();

uint8_t get_seconds_state();
uint16_t get_time();
uint16_t get_state();
uint16_t get_alarm();

#endif
