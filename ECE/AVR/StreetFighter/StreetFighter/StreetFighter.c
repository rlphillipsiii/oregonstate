/*
 * StreetFighter.c
 *
 * Created: 8/25/2012 2:09:05 AM
 * Author:  Robert Phillips III
 *
 * Target:  Tiny261
 * Pin Description:
 *   PB0 - Override signal for the mux in the fpga board
 *   PB1 - Clock signal for the shift register buffer in the fpga board
 *   PB3 - The serial input for the aforementioned shift register buffer
 *   PB4 - The parallel load signal for the output shift register
 *
 * Function Description:
 *   The Tiny261 acts as the controller for SNES when the override
 *   is triggered by 
 */ 


#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

#define OVERRIDE    PB0
#define CLK_OUT     PB1
#define SERIAL_OUT  PB2
#define SHIFT_LATCH PB3

#define BTN1		PINA0
#define BTN2		PINA1
#define BTN3        PINA3
#define BTN4        PINA4
#define DIR_CONTROL PINA6

#define DOWN  (1 << 3)
#define RIGHT (1 << 1)
#define LEFT  (1 << 2)
#define Y     (1 << 7)
#define A     (1 << 0)

#define DFP  0
#define BDK  1
#define FDFP 2
#define BDFP 3

#define RIGHT_DIR 0
#define LEFT_DIR  1

/** 
 * Sets the CPU prescale options
 */
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

#define SETBIT(byte, bit) (byte |= (1 << bit))
#define CLRBIT(byte, bit) (byte &= ~(1 << bit))
#define ISSET(byte, bit)  (byte & (1 << bit))

struct move_sequence {
	unsigned char buttons[10];

	uint8_t sequence_count;
};

/** 
 * Serial out sequence for down on the D-Pad 
 */
unsigned char down = ~DOWN;

/** 
 * Serial out sequence for down+right on the D-Pad 
 */
unsigned char down_right = ~(DOWN | RIGHT);

/** 
 * Serial out sequence for down+left on the D-Pad 
 */
unsigned char down_left = ~(DOWN | LEFT);

/** 
 * Serial out sequence for right on the D-Pad 
 */
unsigned char right = ~RIGHT;

/** 
 * Serial out sequence for left on the D-Pad 
 */
unsigned char left = ~LEFT;

/** 
 * Serial out sequence for Y
 */
unsigned char y_right = ~(Y | RIGHT);

unsigned char y_left = ~(Y | LEFT);

/**
 * Serial out sequence for A
 */
unsigned char a_right = ~(A | RIGHT);

unsigned char a_left = ~(A | LEFT);


volatile unsigned char _override;
volatile unsigned char _latch;

volatile unsigned char dir;
volatile unsigned char move;

volatile struct move_sequence seq;

volatile int16_t cycle;

void enable_INT()
{
    /* trigger external interrupts on the rising edge */
    MCUCR |= (1 << ISC00) | (1 << ISC01);
    
    /* enable INT1 as an external interrupt */
    GIMSK |= (1 << INT1);
}

void setup()
{
    DDRA = 0x00;
    DDRB = 0x0F;
    
    /* enable internal pull-ups on PORTA */
    PORTA = 0xFF;
    
    enable_INT();
    
	dir = RIGHT_DIR;

    SETBIT(PORTB, SHIFT_LATCH);
    sei();
}

void setup_move(volatile struct move_sequence *moves, uint8_t move)
{
	switch (move) {
	case DFP:
		moves->buttons[0] = down;
		moves->sequence_count = 4;

		if (dir == RIGHT_DIR) {
			moves->buttons[1] = down_right;
			moves->buttons[2] = right;
			moves->buttons[3] = y_right;
		} else {
			moves->buttons[1] = down_left;
			moves->buttons[2] = left;
			moves->buttons[3] = y_left;
		}

		break;

	case BDK:
		moves->buttons[0] = down;
		moves->sequence_count = 4;

		if (dir == RIGHT_DIR) {
			moves->buttons[1] = down_left;
			moves->buttons[2] = left;
			moves->buttons[3] = a_left;
		} else {
			moves->buttons[1] = down_right;
			moves->buttons[2] = right;
			moves->buttons[3] = a_right;
		}

		break;

	case FDFP:
		moves->buttons[1] = down;
		moves->sequence_count = 4;

		if (dir == RIGHT_DIR) {
			moves->buttons[0] = right;
			moves->buttons[2] = down_right;
			moves->buttons[3] = y_right;
		} else {
			moves->buttons[0] = left;
			moves->buttons[2] = down_left;
			moves->buttons[3] = y_left;
		}

		break;

	case BDFP:
		moves->buttons[2] = down;
		moves->sequence_count = 5;

		if (dir == RIGHT_DIR) {
			moves->buttons[0] = left;
			moves->buttons[1] = down_left;
			moves->buttons[3] = down_right;
			moves->buttons[4] = y_right;
		} else {
			moves->buttons[0] = right;
			moves->buttons[1] = down_right;
			moves->buttons[3] = down_left;
			moves->buttons[4] = y_left;
		}
	}
}

void latch()
{
	CLRBIT(PORTB, SHIFT_LATCH);
	_delay_us(20);
	
	SETBIT(PORTB, SHIFT_LATCH);
	_delay_us(1);
}

void push_sequence(int clock_cycle, unsigned char buttons)
{
	if (clock_cycle%2 == 0) {
		SETBIT(PORTB, CLK_OUT);
	} else {
		if (clock_cycle < 16) {
			if (ISSET(buttons, (7-clock_cycle/2)))
				SETBIT(PORTB, SERIAL_OUT);
			else
				CLRBIT(PORTB, SERIAL_OUT);
		} else {
			SETBIT(PORTB, SERIAL_OUT);
		}
		
		CLRBIT(PORTB, CLK_OUT);
	}
}

void shift_sequence()
{
    _latch = FALSE;
    _delay_ms(1);
    
    SETBIT(PORTB, SERIAL_OUT);
    for (uint8_t i = 0; i < 32; i++) {
		push_sequence(i, seq.buttons[cycle/3]);
		
		_delay_us(1);
    }
    
    CLRBIT(PORTB, SERIAL_OUT);
    latch();
    SETBIT(PORTB, OVERRIDE);
}

uint8_t debounce_switch(uint16_t *state, uint8_t bit)
{
    *state = (*state << 1) | (!bit_is_clear(PINA, bit)) | 0xE000;
	
    if (*state == 0xF000)
		return 1;

    return 0;
}

char poll_buttons(char reg)
{
    char buttons = 0;

    static uint16_t state[] = {0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t i;
    for (i = 0; i < 8; i++) {
		if (debounce_switch(&state[i], i))
		buttons |= (1 << i);
    }

    return buttons;
}

ISR(INT1_vect)
{
	_latch = TRUE;
	
	if (_override)
		cycle++;
	
	/* secret move button sequence complete */
	if (cycle >= 3*seq.sequence_count || cycle < 0) {
		cycle = 0;
		
		_override = FALSE;
	}
}

int main(void)
{
    CPU_PRESCALE(0);
    
    setup();
    
    while (TRUE) {
		if (_override) {
			if (_latch)
				shift_sequence();
		} else {
			CLRBIT(PORTB, OVERRIDE);
			
			char pina = poll_buttons(PINA);
			if (ISSET(pina, BTN1)) {
				_override = TRUE;

				setup_move(&seq, DFP);
			} else if (ISSET(pina, BTN2)) {
				_override = TRUE;
				
				setup_move(&seq, BDK);
			} else if (ISSET(pina, BTN3)) {
				_override = TRUE;

				setup_move(&seq, FDFP);
			} else if (ISSET(pina, BTN4)) {
				_override = TRUE;
	
				setup_move(&seq, BDFP);
			}else if (ISSET(pina, DIR_CONTROL)) {
				if (dir == RIGHT_DIR)
					dir = LEFT_DIR;
				else
					dir = RIGHT_DIR;
			}

			_delay_ms(1);
		}			
    }	
}
