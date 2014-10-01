#ifndef CLOCK
#define CLOCK

#define ALARM_ENABLED_RADIO  1
#define ALARM_ENABLED_TONE   2
#define ALARM_DISABLED       0

void get_alarm_string(char alarm[11]);

/******************************************************
 * Initializes the clock timer TCNT0 using the
 * external 32kHz oscillator.  The timer is configured
 * to execute an interrupt once every 1/128th of a
 * second.
 *****************************************************/
void init_clock_timer();

/******************************************************
 * Initializes the time that the alarm shows on
 * startup.
 *****************************************************/
void set_time(uint8_t hr, uint8_t min);

/******************************************************
 * Sets or clears a flag that indicates whether or not
 * the alarm is armed.
 *****************************************************/
void set_alarm_state(uint8_t state);

/******************************************************
 * Increments the seconds on the clock by one, which
 * in turn increments the seconds and hours
 * accordingly.
 *****************************************************/
void tick_seconds();

/******************************************************
 * Sets the alarm to go off next at the alarm time
 * plus the number of minutes and seconds given.
 *****************************************************/
void snooze_alarm(uint8_t min, uint8_t sec);

/******************************************************
 * Increments or decrements the clock hour by the
 * number of hours given.
 *****************************************************/
void change_hour(int8_t delta);

/******************************************************
 * Increments or decrements the clock minutes by the
 * number of minutes given.
 *****************************************************/
void change_minute(int8_t delta);

/******************************************************
 * Increments or decrements the alarm clock hour by
 * the number of hours given.
 *****************************************************/
void change_alarm_hour(int8_t delta);

/******************************************************
 * Increments or decrements the alarm clock minute by
 * the number of hours given.
 *****************************************************/
void change_alarm_minute(int8_t delta);

/******************************************************
 * Resets the alarm back to its original value before
 * the alarm clock was snoozed.
 *****************************************************/
void reset_snooze();

/******************************************************
 * Checks to see whether or not the alarm time matches
 * the clock time.
 *****************************************************/
uint8_t is_alarm_time();

/******************************************************
 * Checks to see whether or not the alarm is armed.
 *****************************************************/
uint8_t is_alarm_enabled();

/******************************************************
 * Returns whether or not the number of seconds is odd
 * or even so the colon can be blinked accordingly
 *****************************************************/
uint8_t get_seconds_state(uint8_t mode);

/******************************************************
 * Gets a four digit number corresponding to the time
 * that should be displayed on the clock.
 *****************************************************/
uint16_t get_time();

/******************************************************
 * Gets a four digit number corresponding to the time
 * that the alarm should go off if it is armed.
 *****************************************************/
uint16_t get_alarm();

#endif
