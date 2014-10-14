;***********************************************************
;*
;*	Enter Name of file here
;*
;*	Enter the description of the program here
;*
;*	This is the TRANSMIT skeleton file for Lab 6 of ECE 375
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
.def	val = r17

.equ	EngEnR = 4				; Right Engine Enable Bit
.equ	EngEnL = 7				; Left Engine Enable Bit
.equ	EngDirR = 5				; Right Engine Direction Bit
.equ	EngDirL = 6				; Left Engine Direction Bit

; Use these commands between the remote and TekBot
; MSB = 1 thus:
; commands are shifted right by one and ORed with 0b10000000 = $80
.equ	MovFwd =  ($80|1<<(EngDirR-1)|1<<(EngDirL-1))	;0b10110000 Move Forwards Command
.equ	MovBck =  ($80|$00)								;0b10000000 Move Backwards Command
.equ	TurnR =   ($80|1<<(EngDirL-1))					;0b10100000 Turn Right Command
.equ	TurnL =   ($80|1<<(EngDirR-1))					;0b10010000 Turn Left Command
.equ	Halt =    ($80|1<<(EngEnR-1)|1<<(EngEnL-1))		;0b11001000 Halt Command
.equ	Freez =   ($80|$F8)

.equ	usart_setup = (1 << UCSZ11) | (1 << UCSZ10) | (1 << USBS1)
.equ	BotID =  0b01101111		; Unique XD ID (MSB = 0)
;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;-----------------------------------------------------------
; Interrupt Vectors
;-----------------------------------------------------------
.org	$0000					; Beginning of IVs
		rjmp 	INIT			; Reset interrupt


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

	;I/O Ports
	; Initialize Port D for input
	ldi		mpr, $00
	out		DDRD, mpr

	ldi		mpr, $FF
	out		DDRB, mpr

	;USART1
	; Set the baud rate to 2400bps
	ldi		mpr, $A0
	sts		UBRR1L, mpr

	ldi		mpr, $01
	sts		UBRR1H, mpr

	; Set frame format: 8data bits, 2 stop bit
	ldi		mpr, usart_setup
	sts		UCSR1C, mpr

	; Enable transmitter
	ldi		mpr, (1 << TXEN1)
	sts		UCSR1B, mpr
	;Other


;-----------------------------------------------------------
; Main Program
;-----------------------------------------------------------
MAIN:
		in		val, PIND
		com		val

POLL:
S_8:	mov		mpr, val
		andi	mpr, (1<<7)
		breq	S_7
		ldi		mpr, MovFwd
		rcall	SEND_cmd
		rjmp	MAIN

S_7:	mov		mpr, val
		andi	mpr, (1<<6)
		breq	S_6
		ldi		mpr, MovBck
		rcall	SEND_cmd
		rjmp	MAIN

S_6:	mov		mpr, val
		andi	mpr, (1<<5)
		breq	S_5
		ldi		mpr, TurnR
		rcall	SEND_cmd
		rjmp	MAIN

S_5:	mov		mpr, val
		andi	mpr, (1<<4)
		breq	S_1
		ldi		mpr, TurnL
		rcall	SEND_cmd

S_1:	mov		mpr, val
		andi	mpr, (1<<0)
		breq	NEXT
		ldi		mpr, Freez
		rcall	SEND_cmd

NEXT:	ldi		mpr, Halt
		rcall	SEND_cmd
		rjmp	MAIN

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


SEND_cmd:
		push	mpr

		ldi		mpr, BotId
		rcall	USART_Transmit
		pop		mpr
		out		PORTB, mpr
		rcall	USART_Transmit

;***********************************************************
;*	Stored Program Data
;***********************************************************



;***********************************************************
;*	Additional Program Includes
;***********************************************************