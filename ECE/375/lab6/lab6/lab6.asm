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
.def	dir = r18
.def	tmp = r24
.def	counter = r23			; Counter for loops
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
.equ	BotID =  0b01101111		; Unique XD ID (MSB = 0)
; -----------Change this back!!!-----------------
;.equ	BotID =  0b01101111
; -----------------------------------------------

;/////////////////////////////////////////////////////////////
;These macros are the values to make the TekBot Move.
;/////////////////////////////////////////////////////////////

.equ	MovFwd =  (1<<EngDirR|1<<EngDirL)	;0b01100000 Move Forwards Command
.equ	MovBck =  $00						;0b00000000 Move Backwards Command
.equ	TurnR =   (1<<EngDirL)				;0b01000000 Turn Right Command
.equ	TurnL =   (1<<EngDirR)				;0b00100000 Turn Left Command
.equ	Halt =    (1<<EngEnR|1<<EngEnL)		;0b10010000 Halt Command
.equ	WTime =	100
;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;-----------------------------------------------------------
; Interrupt Vectors
;-----------------------------------------------------------
.org	$0000					; Beginning of IVs
		rjmp 	INIT			; Reset interrupt

.org	$000A
		rjmp	HANDLE_Right

.org	$000C
		rjmp	HANDLE_Left

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

	; I/O Ports
	; Initialize Port B for output
	ldi		mpr, $FF
	out		DDRB, mpr

	; Initialize Port D for input
	ldi		mpr, $00
	out		DDRE, mpr

	ldi		mpr, $FF
	out		PORTE, mpr

	ldi		mpr, $FF
	sts		DDRF, mpr

	ldi		mpr, 0
	sts		PORTF, mpr

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
	ldi		mpr, (1 << RXCIE1 | 1 << RXEN1 | 1 << TXEN1)
	sts		UCSR1B, mpr

	; External Interrupts
	ldi		mpr, (1 << ISC51 | 1 << ISC41)
	sts		EICRB, mpr

	ldi		mpr, (1 << INT4 | 1 << INT5)
	out		EIMSK, mpr

	sei


;-----------------------------------------------------------
; Main Program
;-----------------------------------------------------------
MAIN:
		ldi		dir, Halt
		out		PORTB, dir

WAITING:
		rjmp	WAITING

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


USART_Receive:
		lds		mpr, UDR1
		cpi		cmd, 1
		breq	FWD

		cpi		mpr, BotId
		brne	FRZ
		ldi		cmd, 1
		rjmp	END

FRZ:	cpi		mpr, 0b11111000
		brne	END
		in		mpr, PINB
		push	mpr
		ldi		mpr, Halt
		out		PORTB, mpr

		; take this part out!!!!-----
		;ldi		mpr, 0b11111111
		;out		PORTB, mpr
		;-----------------------------

		ldi		waitcnt, WTime
		rcall	Wait
		ldi		waitcnt, WTime
		rcall	Wait
		pop		mpr
		out		PORTB, mpr

END:	reti

		; switch statement
FWD:	cpi		mpr, 0b10110000
		brne	REV
		ldi		dir, MovFwd
		out		PORTB, dir
		rjmp	DEFAULT

REV:	cpi		mpr, 0b10000000
		brne	LEFT
		ldi		dir, MovBck
		out		PORTB, dir
		rjmp	DEFAULT

LEFT:	cpi		mpr, 0b10010000
		brne	RGHT
		ldi		dir, TurnL
		out		PORTB, dir
		rjmp	DEFAULT

RGHT:	cpi		mpr, 0b10100000
		brne	STOP
		ldi		dir, TurnR
		out		PORTB, dir
		rjmp	DEFAULT

STOP:	cpi		mpr, 0b11001000
		brne	SEND_f
		ldi		dir, Halt
		out		PORTB, dir
		rjmp	DEFAULT

SEND_f:	cpi		mpr, 0b11111000
		brne	DEFAULT
		ldi		mpr, Halt
		rcall	USART_Transmit

DEFAULT:
		clr		cmd
		reti


HANDLE_Left:
		in		tmp, PINB

		; Move Backwards for a second
		ldi		mpr, MovBck		; Load Move Backwards command
		out		PORTB, mpr		; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Turn left for a second
		ldi		mpr, TurnL		; Load Turn Left Command
		out		PORTB, mpr		; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		out		PORTB, tmp

		ldi		mpr, $FF
		out		EIFR, mpr
		reti


HANDLE_Right:
		in		tmp, PORTB

		; Move Backwards for a second
		ldi		mpr, MovBck		; Load Move Backwards command
		out		PORTB, mpr		; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		; Turn right for a second
		ldi		mpr, TurnR		; Load Turn Left Command
		out		PORTB, mpr		; Send command to port
		ldi		waitcnt, WTime	; Wait for 1 second
		rcall	Wait			; Call wait function

		out		PORTB, tmp

		ldi		mpr, $FF
		out		EIFR, mpr
		reti


Wait:
		push	waitcnt			; Save wait register
		push	ilcnt			; Save ilcnt register
		push	olcnt			; Save olcnt register

Loop:	ldi		olcnt, 224		; load olcnt register
OLoop:	ldi		ilcnt, 237		; load ilcnt register
ILoop:	dec		ilcnt			; decrement ilcnt
		brne	ILoop			; Continue Inner Loop
		dec		olcnt		; decrement olcnt
		brne	OLoop			; Continue Outer Loop
		dec		waitcnt		; Decrement wait 
		brne	Loop			; Continue Wait loop	

		pop		olcnt		; Restore olcnt register
		pop		ilcnt		; Restore ilcnt register
		pop		waitcnt		; Restore wait register
		ret				; Return from subroutine
;***********************************************************
;*	Stored Program Data
;***********************************************************



;***********************************************************
;*	Additional Program Includes
;***********************************************************
