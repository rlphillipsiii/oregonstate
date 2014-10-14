;***********************************************************
;*
;*	Enter Name of file here
;*
;*	Enter the description of the program here
;*
;*	This is the skeleton file Lab 3 of ECE 375
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

.equ	LCDLast = $011F
.equ	LCDSecond = $0101

;***********************************************************
;*	Start of Code Segment
;***********************************************************
.cseg							; Beginning of code segment

;-----------------------------------------------------------
; Interrupt Vectors
;-----------------------------------------------------------
.org	$0000					; Beginning of IVs
		rjmp INIT				; Reset interrupt

.org	$0046					; End of Interrupt Vectors

;-----------------------------------------------------------
; Program Initialization
;-----------------------------------------------------------
INIT:
		; Initialize Stack Pointer
		ldi		mpr, low(RAMEND)
		out		SPL, mpr
		ldi		mpr, high(RAMEND)
		out		SPH, mpr

		; Initialize LCD Display
		rcall	LCDInit
		
		; NOTE that there is no RET or RJMP from INIT, this is
		; because the next instruction executed is the first for
		; the main program

;-----------------------------------------------------------
; Main Program
;-----------------------------------------------------------
MAIN:
		; Display the strings on the LCD Display
		ldi		ZL, low(NAME << 1)
		ldi		ZH, high(NAME << 1)
		rcall	WRITE_TOP
		
		ldi		ZL, low(BIZ << 1)
		ldi		ZH, high(BIZ << 1)
		rcall	WRITE_BOTTOM

DISPLAY_LOOP:
		in		mpr, PIND
		andi	mpr, (1 << 0)
		brne	MAIN_LOOP

MAIN_LOOP:
		in		mpr, PIND
		andi	mpr, (1 << 1)
		brne	MAIN

		rcall	SCROLL		; move the characters in memory
		rcall	LCDWrLn1	; echo the changes to the top row
		rcall	LCDWrLn2	; echo the changes to the bottom row

		ldi		waitcnt, 15	; set the wait to 150 ms
		rcall	DELAY		; wait

		rjmp	MAIN_LOOP	; loop again

;***********************************************************
;*	Functions and Subroutines
;***********************************************************

;-----------------------------------------------------------
; Func: Template function header
; Desc: Cut and paste this and fill in the info at the 
;		beginning of your functions
;-----------------------------------------------------------
WRITE_TOP:
		; Save variable by pushing them to the stack
		push mpr
		push counter

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

		rcall	LCDWrLn1

		; Restore variable by popping them from the stack in reverse order\
		pop counter
		pop mpr

		ret	


WRITE_BOTTOM:
		; Save variable by pushing them to the stack
		push mpr
		push counter

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

		rcall LCDWrLn2

		; Restore variable by popping them from the stack in reverse order\
		pop counter
		pop mpr

		ret	

SCROLL:
		push counter
		push mpr

		ldi		counter, 32

		; push the last character onto the stack
		ldi		ZL, low(LCDLast)
		ldi		ZH, high(LCDLast)
		ld		mpr, Z
		push	mpr
		
		; load Y (destination) with the address of the
		; second character
		ldi		YL, low(LCDLast + 1)
		ldi		YH, high(LCDLast + 1)

		; copy all the characters
SCRL_LOOP:
		; while the count != 0, swap characters with the one in
		; front of it
		ld		mpr, -Z
		st		-Y, mpr
		dec		counter
		brne	SCRL_LOOP

		; put the last character in the first position
		pop		mpr
		st		Y, mpr

		pop mpr
		pop counter

		ret

DELAY:
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

;----------------------------------------------------------
; An example of storing a string, note the preceeding and
; appending labels, these help to access the data
;----------------------------------------------------------
STRING_BEG:
.DB		"My Test String"		; Storing the string in Program Memory

NAME:
.DB		"   BIG ROB      "

BIZ:
.DB		"  PRODUCTIONS   "

;***********************************************************
;*	Additional Program Includes
;***********************************************************
.include "LCDDriver.asm"		; Include the LCD Driver
