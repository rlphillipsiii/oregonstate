;***********************************************************
;*
;*	Enter Name of file here
;*
;*	Enter the description of the program here
;*
;*	This is the skeleton file Lab 5 of ECE 375
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
.def	mpr = r16				; Multipurpose register required for LCD Driver
.def	counter = r23			; Counter for loops
.def	waitcnt = r17			; Wait Loop Counter
.def	tmp = r15				; Temporary register
.def	ilcnt = r18				; Inner Loop Counter
.def	olcnt = r19				; Outer Loop Counter

.def	memory = r20
.def	last = r21

.equ	WTime = 100
.equ	EngEnR = 4				; Right Engine Enable Bit
.equ	EngEnL = 7				; Left Engine Enable Bit
.equ	EngDirR = 5				; Right Engine Direction Bit
.equ	EngDirL = 6				; Left Engine Direction Bit
.equ	Right = 1
.equ	Left = 2

;/////////////////////////////////////////////////////////////
;These macros are the values to make the TekBot Move.
;/////////////////////////////////////////////////////////////

.equ	MovFwd = (1<<EngDirR|1<<EngDirL)	; Move Forwards Command
.equ	MovBck = $00				; Move Backwards Command
.equ	TurnR = (1<<EngDirL)			; Turn Right Command
.equ	TurnL = (1<<EngDirR)			; Turn Left Command
.equ	Halt = (1<<EngEnR|1<<EngEnL)		; Halt Command

; Using the constants from above, create the movement 
; commands, Forwards, Backwards, Stop, Turn Left, and Turn Right

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
		rjmp	HANDLE_RIGHT

.org	$0004
		rjmp	HANDLE_LEFT

.org	$0046					; End of Interrupt Vectors

;-----------------------------------------------------------
; Program Initialization
;-----------------------------------------------------------
INIT:	; The initialization routine

		; Initialize the stack pointer
		ldi		mpr, low(RAMEND)
		out		SPL, mpr
		ldi		mpr, high(RAMEND)
		out		SPH, mpr

		; Initialize Port B for output
		ldi		mpr, $FF
		out		DDRB, mpr

		; Initialize Port D for input
		ldi		mpr, $00
		out		DDRD, mpr

		; Initialize external interrupts
		ldi		mpr, (1 << ISC01 | 1 << ISC11)
		sts		EICRA, mpr

		ldi		mpr, (1 << INT1 | 1 << INT0)
		out		EIMSK, mpr

		; Turn on interrupts
		sei

;-----------------------------------------------------------
; Main Program
;-----------------------------------------------------------
MAIN:	; The Main program

		ldi		mpr, MovFwd	; Load Move Forwards command
		out		PORTB, mpr	; Send command to port

		rjmp	MAIN			; Create an infinite while loop to signify the 
								; end of the program.

;***********************************************************
;*	Functions and Subroutines
;***********************************************************

;-----------------------------------------------------------
; You will probably need several functions, one to handle the 
; left whisker interrupt, one to handle the right whisker 
; interrupt, and maybe a wait function
;------------------------------------------------------------

;-----------------------------------------------------------
; Func: Template function header
; Desc: Cut and paste this and fill in the info at the 
;		beginning of your functions
;-----------------------------------------------------------
HANDLE_RIGHT:
		cpi		last, Left
		breq	INC_RIGHT
		ldi		memory, 0
		rjmp	CHECK_RIGHT

INC_RIGHT:
		inc		memory

CHECK_RIGHT:
		cpi		memory, 5
		brne	TURN_RIGHT
		rcall	SPIN_180
			
		ldi		memory, 0
		rjmp	FINISH_RIGHT

TURN_RIGHT:
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

		cpi		last, Right
		brne	FINISH_RIGHT
		ldi		waitcnt, WTime
		rcall	Wait

FINISH_RIGHT:
		ldi		mpr, $FF
		out		EIFR, mpr

		ldi		last, Right

		reti					; Return from subroutine


HANDLE_LEFT:
		cpi		last, Right
		breq	INC_LEFT
		ldi		memory, 0
		rjmp	CHECK_LEFT

INC_LEFT:
		inc		memory

CHECK_LEFT:
		cpi		memory, 5
		brne	TURN_LEFT
		rcall	SPIN_180
			
		ldi		memory, 0
		rjmp	FINISH_LEFT

TURN_LEFT:
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

		cpi		last, Left
		brne	FINISH_LEFT
		ldi		waitcnt, WTime
		rcall	Wait

FINISH_LEFT:
		ldi		mpr, $FF
		out		EIFR, mpr

		ldi		last, Left

		reti					; Return from subroutine


SPIN_180:
		ldi		mpr, Halt
		out		PORTB, mpr
		ldi		waitcnt, WTime
		rcall	Wait

		ldi		mpr, TurnR
		out		PORTB, mpr
		ldi		waitcnt, 200
		rcall	Wait

		ret

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

; Enter any stored data you might need here

;***********************************************************
;*	Additional Program Includes
;***********************************************************
; There are no additional file includes for this program
