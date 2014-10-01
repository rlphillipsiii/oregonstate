#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include "led_driver.h"
#include "macros.h"

/* holds the clock value */
static volatile uint8_t hour;
static volatile uint8_t minutes;
static volatile uint8_t seconds;

/* holds the value of the alarm that was set */
static volatile uint8_t a_hour;
static volatile uint8_t a_minutes;
static volatile uint8_t a_seconds;

/* holds the value of the actual next alarm including snooze */
static volatile uint8_t s_hour;
static volatile uint8_t s_minutes;
static volatile uint8_t s_seconds;

static volatile uint8_t alarm_armed;

void change_hour(int8_t delta)
{
	seconds = 0;

	if (hour+delta == 24)
		hour = 0;
	else if (hour+delta+1 == 0)
		hour = 23;
	else
		hour += delta;
}

void change_minute(int8_t delta)
{
	seconds = 0;

	if (minutes+delta == 60)
		minutes = 0;
	else if (minutes+delta+1 == 0)
		minutes = 59;
	else
		minutes += delta;
}

void change_alarm_hour(int8_t delta)
{
	a_seconds = 0;
	s_seconds = 0;

	if (a_hour+delta == 24)
		a_hour = 0;
	else if (a_hour+delta+1 == 0)
		a_hour = 23;
	else
		a_hour += delta;

	s_hour = a_hour;
}

void change_alarm_minute(int8_t delta)
{
	a_seconds = 0;
	s_seconds = 0;
	
	if (a_minutes+delta == 60)
		a_minutes = 0;
	else if (a_minutes+delta+1 == 0)
		a_minutes = 59;
	else
		a_minutes += delta;

	s_minutes = a_minutes;
}

ISR(TIMER0_COMP_vect)
{
	seconds++;
	if (seconds == 60) {
		seconds = 0;
		
		minutes++;
		if (minutes == 60) {
			minutes = 0;
			
			hour++;
			if (hour == 24)
				hour = 0;
		}
	}

	  
	TCNT0 = 0;
}

void init_clock_timer()
{
	hour = 0;
	minutes = 0;
	seconds = 0;

	a_hour = 0;
	a_minutes = 0;
	a_seconds = 0;

	alarm_armed = FALSE;

	ASSR = (1 << AS0);
	TIMSK |= (1 << OCIE0);

	TCCR0 |= (1 << CS02) | (1 << CS01);
	OCR0 = 125;
}

void set_time(uint8_t hr, uint8_t min)
{
	hour = hr;
	minutes = min;

	seconds = 0;
}

void set_alarm(uint8_t hr, uint8_t min)
{
	a_hour = hr;
	s_hour = hr;
	
	a_minutes = min;
	s_minutes = min;
	
	a_seconds = 0;
	s_seconds = 0;
}

void snooze_alarm(uint8_t mins, uint8_t sec)
{
	s_seconds += sec;
	if (s_seconds > 59) {
		s_seconds -= 60;
		s_minutes++;
	}

	s_minutes += mins;
	if (s_minutes > 59) {
		s_minutes -= 60;

		s_hour++;
		if (s_hour == 24)
			s_hour = 0;
	}
}

uint16_t get_time()
{
	return hour*100+minutes;
}

uint16_t get_alarm()
{
	return a_hour*100+a_minutes;
}

uint8_t get_seconds_state()
{
	return seconds%2;
}

void set_alarm_state(uint8_t state)
{
	alarm_armed = state;
}

uint8_t is_alarm_time()
{
	if (!alarm_armed)
		return FALSE;

	if (seconds == s_seconds &&
		minutes == s_minutes &&
		hour == s_hour)
		return TRUE;

	return FALSE;
}
