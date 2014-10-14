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
.def	tmp = r21
.def	recv = r20
.def	counter = r23			; Counter for loops
.def	stat = r24
.def	waitcnt = r17			; Wait Loop Counter
.def	tmp = r15				; Temporary register
.def	ilcnt = r18				; Inner Loop Counter
.def	olcnt = r19				; Outer Loop Counter

.equ	WskrR = 0				; Right Whisker Input Bit
.equ	WskrL = 1				; Left Whisker Input Bit
.equ	EngEnR = 4				; Right Engine Enable Bit
.equ	EngEnL = 7				; Left Engine Enable Bit
.equ	EngDirR = 5				; Right Engine Direction Bit
.equ	EngDirL = 6				; Left Engine Direction Bit
.equ	usart_setup = (1 << UCSZ11) | (1 << UCSZ10) | (1 << USBS1)

.equ	sc_disp = (SCORE_TXT) + 7
.equ	tm_disp = (TIME_LEFT) + 11
.equ	ASCII = $0130

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

.org	$003C
		rjmp	USART_Receive

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
	
	rcall	INIT_TOP
	rcall	INIT_BOTTOM

	rcall	LCDWrite

	; USART1
	; Set the baud rate to 2400bps
	ldi		mpr, $A0
	sts		UBRR1L, mpr

	ldi		mpr, $01
	sts		UBRR1H, mpr

	; Set frame format: 8data bits, 2 stop bit
	ldi		mpr, usart_setup
	sts		UCSR1C, mpr

	; Enable receiver and enable receive interrupts
	ldi		mpr, (1 << RXCIE1 | 1 << RXEN1)
	sts		UCSR1B, mpr

	sei


;-----------------------------------------------------------
; Main Program
;-----------------------------------------------------------
MAIN:	rjmp	MAIN

;***********************************************************
;*	Functions and Subroutines
;***********************************************************
USART_Receive:
		lds		mpr, UDR1

		cpi		stat, 1
		breq	RECV_Time

		cpi		stat, 2
		breq	RECV_Score

		cpi		mpr, time
		brne	CHK
		ldi		stat, 1
		reti
CHK:	cpi		mpr, score
		brne	RETURN
		ldi		stat, 2
RETURN:	reti

RECV_Score:
		rcall	INIT_BOTTOM

		ldi		YL, low(LCDLn2Addr+8)
		ldi		YH, high(LCDLn2Addr+8)

		ldi		XL, low(ASCII)
		ldi		XH, high(ASCII)
		clr		count
		rcall	Bin2Ascii

		rjmp	SHOW

RECV_Time:
		cpi		mpr, 0
		brne	CONT
		rcall	END_GAME
		rcall	LCDWrLn1
		reti

CONT:	rcall	INIT_TOP

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

		clr		stat
		reti


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
