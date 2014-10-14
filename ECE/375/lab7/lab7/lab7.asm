;***********************************************************
;*
;*	Enter Name of file here
;*
;*	Enter the description of the program here
;*
;*	This is the RECEIVE skeleton file for Lab 6 of ECE 375
;*
;***********************************************************
;*
;*	 Author: Enter your name
;*	   Date: Enter Date
;*
;***********************************************************

.include "m128def.inc"			; Include definition file

;***********************************************************
;*	Internal Register Definitions and Constants
;***********************************************************
.def	mpr = r16				; Multi-Purpose Register
.def	cmd = r21
.def	timer = r24
.def	counter = r23			; Counter for loops
.def	score_reg = r3
.def	tmp = r15				; Temporary register
.def	ilcnt = r18				; Inner Loop Counter
.def	olcnt = r19				; Outer Loop Counter
.def	val = r17

.equ	usart_setup = (1 << UCSZ11) | (1 << UCSZ10) | (1 << USBS1)
.equ	ASCII = $0130
.equ	WTime =	100
.equ	score = 0b01110011
.equ	time = 0b01110100
;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;-----------------------------------------------------------
; Interrupt Vectors
;-----------------------------------------------------------
.org	$0000					; Beginning of IVs
		rjmp 	INIT			; Reset interrupt

.org	$0002
		rjmp	HANDLE_INT

.org	$000A
		rjmp	HANDLE_INT

.org	$0018
		rjmp	HANDLE_OCI0

.org	$0046					; End of Interrupt Vectors

;-----------------------------------------------------------
; Program Initialization
;-----------------------------------------------------------
INIT:
	; Stack Pointer (VERY IMPORTANT!!!!)
	ldi		mpr, low(RAMEND)
	out		SPL, mpr
	ldi		mpr, high(RAMEND)
	out		SPH, mpr

	rcall	LCDInit

	; I/O Ports
	; Initialize Port B for output
	ldi		mpr, $FF
	out		DDRB, mpr

	; Initialize Port D for input
	ldi		mpr, $00
	out		DDRE, mpr
	out		DDRD, mpr

	ldi		mpr, $FF
	out		PORTE, mpr
	out		PORTD, mpr

S_8:	in		val, PIND
		com		val

		mov		mpr, val
		andi	mpr, (1<<7)
		breq	S_8

	; USART1
	; Set the baud rate to 2400bps
	ldi		mpr, $01
	sts		UBRR1H, mpr

	ldi		mpr, $A0
	sts		UBRR1L, mpr

	; Set frame format: 8data bits, 2 stop bit
	ldi		mpr, usart_setup
	sts		UCSR1C, mpr

	; Enable receiver and enable receive interrupts
	ldi		mpr, (1 << TXEN1)
	sts		UCSR1B, mpr

	; Set prescaler for OCR0 to divide by 1024
	ldi		mpr, (1 << CS12 | 1 << CS10)
	out		TCCR1B, mpr

	; Set the top of the counter to 16
	ldi		mpr, $3D
	out		OCR1AH, mpr

	ldi		mpr, $09
	out		OCR1AL, mpr

	; initialize TCNT0 to 0
	clr		mpr
	out		TCNT1H, mpr
	out		TCNT1L, mpr

	; Enable timer counter compare match interrupt 0
	ldi		mpr, 1 << OCIE1A
	out		TIMSK, mpr

	; External Interrupts
;	ldi		mpr, (1 << ISC41 | 1 << ISC40)
	ldi		mpr, (1 << ISC41)
	sts		EICRB, mpr

	ldi		mpr, (1 << ISC01 | 1 << ISC11)
	sts		EICRA, mpr

	ldi		mpr, (1 << INT4 | 1 << INT0)
	out		EIMSK, mpr

	clr		score_reg
	ldi		timer, 60
	
	rcall	INIT_TOP
	rcall	INIT_BOTTOM
	rcall	LCDWrite

	sei


;-----------------------------------------------------------
; Main Program
;-----------------------------------------------------------
MAIN:
		ldi		mpr, score
		rcall	USART_Transmit

		clr		score_reg
		mov		mpr, score_reg
		rcall	USART_Transmit

WAITING:	rjmp	WAITING

;***********************************************************
;*	Functions and Subroutines
;***********************************************************
USART_Transmit:
		push	mpr
POLL_Wr:
		lds		mpr, UCSR1A
		sbrs	mpr, UDRE1
		rjmp	POLL_Wr
		
		pop		mpr
		sts		UDR1, mpr
		ret


HANDLE_INT:
		inc		score_reg
		mov		mpr, score_reg

		rcall	INIT_BOTTOM

		ldi		YL, low(LCDLn2Addr+8)
		ldi		YH, high(LCDLn2Addr+8)

		ldi		XL, low(ASCII)
		ldi		XH, high(ASCII)
		clr		count
		rcall	Bin2Ascii

SHOW2:	ld		mpr, X+
		st		Y+, mpr
		dec		count
		brne	SHOW2

		rcall	LCDWrite

		ldi		mpr, score
		rcall	USART_Transmit

		mov		mpr, score_reg
		rcall	USART_Transmit

		ldi		wait, 25
		rcall	Wait_Time

		ldi		mpr, $FF
		out		EIFR, mpr
		reti


HANDLE_OCI0:
		cpi		timer, 0
		brne	STOP
		reti

STOP:	dec		timer
		cpi		timer, 0
		brne	SEND
		ldi		mpr, 0
		out		EIMSK, mpr
		out		TIMSK, mpr

SEND:	rcall	LCDClrLn1
		mov		mpr, timer

		rcall	INIT_TOP

		ldi		YL, low(LCDLn1Addr+11)
		ldi		YH, high(LCDLn1Addr+11)

		ldi		XL, low(ASCII)
		ldi		XH, high(ASCII)
		clr		count
		rcall	Bin2Ascii

SHOW:	ld		mpr, X+
		st		Y+, mpr
		dec		count
		brne	SHOW
		
		rcall	LCDWrite
	
		ldi		mpr, time
		rcall	USART_Transmit

		mov		mpr, timer
		rcall	USART_Transmit

		clr		mpr
		out		TCNT1H, mpr
		out		TCNT1L, mpr

		reti

Wait_Time:
		push	wait			; Save wait register
		push	ilcnt			; Save ilcnt register
		push	olcnt			; Save olcnt register

Loop:	ldi		olcnt, 224		; load olcnt register
OLoop:	ldi		ilcnt, 237		; load ilcnt register
ILoop:	dec		ilcnt			; decrement ilcnt
		brne	ILoop			; Continue Inner Loop
		dec		olcnt		; decrement olcnt
		brne	OLoop			; Continue Outer Loop
		dec		wait		; Decrement wait 
		brne	Loop			; Continue Wait loop	

		pop		olcnt		; Restore olcnt register
		pop		ilcnt		; Restore ilcnt register
		pop		wait		; Restore wait register
		ret				; Return from subroutine


INIT_TOP:
		; Save variable by pushing them to the stack
		push mpr
		push counter

		ldi		ZL, low(TIME_LEFT << 1)
		ldi		ZH, high(TIME_LEFT << 1)

		rcall	LCDClrLn1

		; Execute the function here
		ldi		YL,	low(LCDLn1Addr)
		ldi		YH,	high(LCDLn1Addr)
		ldi		counter, LCDMaxCnt
LINE1_LOOP:
		lpm		mpr, Z+
		st		Y+, mpr
		dec		counter
		brne	LINE1_LOOP

		; Restore variable by popping them from the stack in reverse order\
		pop counter
		pop mpr

		ret	


END_GAME:
		; Save variable by pushing them to the stack
		push mpr
		push counter

		ldi		ZL, low(GAME_OVER << 1)
		ldi		ZH, high(GAME_OVER << 1)

		rcall	LCDClrLn1

		; Execute the function here
		ldi		YL,	low(LCDLn1Addr)
		ldi		YH,	high(LCDLn1Addr)
		ldi		counter, LCDMaxCnt
LINE10_LOOP:
		lpm		mpr, Z+
		st		Y+, mpr
		dec		counter
		brne	LINE10_LOOP

		; Restore variable by popping them from the stack in reverse order\
		pop counter
		pop mpr

		ret	


INIT_BOTTOM:
		; Save variable by pushing them to the stack
		push mpr
		push counter

		ldi		ZL, low(SCORE_TXT << 1)
		ldi		ZH, high(SCORE_TXT << 1)

		rcall	LCDClrLn2

		; Execute the function here
		ldi		YL,	low(LCDLn2Addr)
		ldi		YH,	high(LCDLn2Addr)
		ldi		counter, LCDMaxCnt
LINE2_LOOP:
		lpm		mpr, Z+
		st		Y+, mpr
		dec		counter
		brne	LINE2_LOOP

		; Restore variable by popping them from the stack in reverse order\
		pop counter
		pop mpr

		ret	

;***********************************************************
;*	Stored Program Data
;***********************************************************
SCORE_TXT:
.db		"Score:  0       "

TIME_LEFT:
.db		"Time Left:      "

GAME_OVER:
.db		"Game Over!      "

;***********************************************************
;*	Additional Program Includes
;***********************************************************
.include "LCDDriver.asm"
