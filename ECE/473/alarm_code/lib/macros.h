#ifndef MACROS
#define MACROS

#define TRUE  1
#define FALSE 0

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define BUTTON4 3
#define BUTTON5 4
#define BUTTON6 5
#define BUTTON7 6
#define BUTTON8 7

#define MODE_DEFAULT      0
#define MODE_SET_TIME     1
#define MODE_SET_ALARM    2
#define MODE_ENABLE_ALARM 3
#define MODE4 4
#define MODE5 5
#define MODE_TUNE_RADIO   6
#define MODE7 7
#define MODE8 8

/******************************************************
 * Checks to see if the given bit is set.
 ******************************************************/
#define ISSET(byte, bit) (byte & (1 << bit))

/******************************************************
 * Sets the given bit.
 ******************************************************/
#define SETBIT(byte, bit) (byte |= (1 << bit))

/******************************************************
 * Clears the given bit.
 ******************************************************/
#define CLRBIT(byte, bit) (byte &= ~(1 << bit))

/******************************************************
 * Toggles the given bit.
 ******************************************************/
#define TOGGLEBIT(byte, bit) (byte ^= (1 << bit))

#endif
