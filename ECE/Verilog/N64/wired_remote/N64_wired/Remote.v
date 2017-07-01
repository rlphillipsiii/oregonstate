`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 	TekBots
// Engineer: 	Robert L. Phillips III
// 
// Create Date:    12:21:01 07/18/2012 
// Design Name: 	 N64 Controller Decoder
// Module Name:    Remote 
// Project Name: 	 N64 Controlled TekBot
// Target Devices: OSU TekBots D Logic Board
// Tool versions: 
// Description: 
//						
//	N64 Controller Protocol:
//			In order to get the status of the N64 buttons, a 9 bit signal must be
//			sent to the controller over the data line.  This signal contains 7 0's
//			followed by 2 1's (8 bit signal and a stop bit).  The controller then
//			immediately responds over the same data line with 33 bits.  The first
//			16 bits correspond to the buttons and the second 16 bits correspond to
//			the joystick.  The last bit is a stop bit.
//
// N64 Asynchronous Serial Comm:
//			Because the N64 controller and system operate off two asynchronous 
//			1 MHz clocks, it is necessary for the two to utilize a series of 4 sub-
//			bits to represent 0's and 1's instead of a simple high and low system.
//			Each sub-bit lasts for 1 us, which indicates that it takes 4 us to
//			transmit one bit of information (250 kHz).  Each sub bit starts with a
//			low start sub-bit and ends with a high sub-bit.  The middle two sub-bits
//			correspond to the true value of the bit.
//																					_
//			0 (3 low sub-bits followed by 1 high sub-bit) --> ___|
//               															 ___
//			1 (1 low sub-bit followed by 3 high sub-bits) --> _|
//
// N64 Response Bits (1 - 16):
//			1			2			3			4
//			A			B			Z			Start	
//			
//			5			6			7			8
//			Up			Down		Left		Right	
//
//			9			10			11			12
//			None		None		L			R	
//
//			13			14			15			16
//			C-Up		C-Down	C-Left	C-Right
//
// Clock Signals:
//			Four clocks are necessary for the logic:  1 MHz, 500 kHz, 250 kHz, 1 kHz.
//			The first three are derived by dividing the 2 MHz clock that is used internally
//			by the N64 controller.  The 1 kHz clock is the clock used by the CPLD and is
//			only used to transmit the decoded controller bits.
//
// Dependencies: N64 Controller
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Remote(
    input Clk,
	 input Internal_Clk,
    input Global_Reset,
	 output IR_Xmit,
    output Len,
    output Ldir,
    output Ren,
    output Rdir,
	 inout Serial_IO
    );

	wire Clock_2, Clock_4, Clock_8, serial_out, low, high, 
			signal, select, latch, ir_out, transmit;
				
	wire [15:0] parallel_out, shift_out;
	wire [3:0] count_4, dirs;
	wire [1:0] count_2;

	assign Serial_IO = serial_out;
	
	assign Len = dirs[3];
	assign Ldir = dirs[2];
	assign Ren = dirs[1];
	assign Rdir = dirs[0];
	assign IR_Xmit = ir_out;
	
	ClockDivider clock_divider (
		 .Clock(Clk), 
		 .Reset(Global_Reset), 
		 .Clock_2(Clock_2), 
		 .Clock_4(Clock_4), 
		 .Clock_8(Clock_8)
		 );
		 
	TwoBitCounter TBC(
		.clk(Clock_2),
		.reset(Global_Reset),
		.count(count_2)
		);
		
	FourBitCounter FBC(
		.clk(Clock_8),
		.reset(Global_Reset),
		.ext_reset(count_reset),
		.count(count_4)
		);
		
	LogicHigh LH(
		.count(count_2),
		.out(high)
		);
		
	LogicLow LL(
		.count(count_2),
		.out(low)
		);
		
	SignalGenerator SG(
		.count(count_4),
		.logic_low(low),
		.logic_high(high),
		.signal(signal)
		);
		
	Mux Multiplexer(
		.serial(signal),
		.select(select),
		.out(serial_out)
		);
		
	IOSelect IOS(
		.clk(Clock_4),
		.reset(Global_Reset),
		.count(),
		.transmit(transmit),
		.latch(latch),
		.count_reset(count_reset),
		.select(select)
		);

	ShiftRegister SR (
		.reset(Global_Reset),
		.clk(Clock_8),
		.serial_in(Serial_IO),
		.buffer(shift_out)
		);
		
	FlipFlop FF (
		.reset(Global_Reset),
		.latch(latch),
		.in(shift_out),
		.out(parallel_out)
		);
		
	Decoder Decode(
		.state(parallel_out),
		.dirs(dirs)
		);
	
	IRTransmitter IRT (
		.reset(Global_Reset),
		.dirs(dirs),
		.trans(transmit),
		.clk(Internal_Clk),
		.serial_out(ir_out)
		);
endmodule


/*
 * Generates the 8 bit button status signal and stop bit that polls the controller.
 * The signal generator is simply a mux that uses the count from the 4-bit counter as
 * its select.
 */
module SignalGenerator (
	input [3:0] count,
	input logic_low,
	input logic_high,
	output reg signal
	);
	
	always @(*)
		case(count)
			4'b0111:	signal <= logic_high;
			4'b1000: signal <= logic_high;
			default:	signal <= logic_low;
		endcase
		
endmodule


/* 
 * Continually samples the serial line and shifts the bits to the left.  The
 * shift register data is only sampled by the Flip Flop after the first 16 bits 
 * from the controller have been loaded into the shift register.
 */
module ShiftRegister (
	input clk,
	input reset,
	input serial_in,
	output reg [15:0] buffer
	);
			
	always @(negedge clk, negedge reset)
		if(!reset)	buffer <= 16'b0;
		else 			buffer <= {buffer[14:0], serial_in};

endmodule


/* 
 * Decodes the signal from the Flip Flop and drives the motor accordingly.
 */
module Decoder (
	input [15:0] state,
	output reg [3:0] dirs
	);
	
	always @(*)
		case(state & 16'b1111_1111_0011_1111)
			16'b0000_1000_0000_0000:   	dirs <= 4'b0101;  //forward
			16'b0000_0100_0000_0000:   	dirs <= 4'b0000;  //reverse
			16'b0000_0001_0000_0000:   	dirs <= 4'b0001;  //spin right
			16'b0000_0010_0000_0000:   	dirs <= 4'b0100;  //spin left
			16'b0000_1010_0000_0000:   	dirs <= 4'b0110;  //veer left
			16'b0000_1001_0000_0000:   	dirs <= 4'b1001;  //veer right
			16'b0000_0110_0000_0000:   	dirs <= 4'b1000;  //back veer left
			16'b0000_0101_0000_0000:   	dirs <= 4'b0010;  //back veer right
			default:								dirs <= 4'b1111;	//stop
		endcase
		
endmodule


/*
 * Latches onto the parallel output of the shift register when the latch input
 * goes high.  The latch signal comes from the IO Select.
 */
module FlipFlop (
	input [15:0] in,
	input reset,
	input latch,
	output reg [15:0] out
	);
	
	always @(posedge latch, negedge reset)
		if(!reset)	out <= 16'b0;
		else			out <= in;
		
endmodule


/*
 * Transmits the decoded direction bits over the IR preceded by the start sequence
 * 10100 that is needed to communicate to the receiver that the direction bits are going
 * to be transmitted.  Operates off the 1 kHz clock.
 */
module IRTransmitter (
	input reset,
	input clk,
	input trans,
	input [3:0] dirs,
	output reg serial_out
	);
	
	reg [3:0] state, next;
	
	parameter S0 = 4'b0000;
	parameter S1 = 4'b0001;
	parameter S2 = 4'b0010;
	parameter S3 = 4'b0011;
	parameter S4 = 4'b0100;
	parameter S5 = 4'b0101;
	parameter S6 = 4'b0110;
	parameter S7 = 4'b0111;
	parameter S8 = 4'b1000;
	parameter S9 = 4'b1001;
	
	always @(posedge clk, negedge reset)
		if(!reset)	state <= S0;
		else			state <= next;
		
	always @(posedge clk, negedge reset)
		if(!reset)	serial_out <= 1;
		else
			case(state)
				S0:			serial_out <= 1;
				S1:			serial_out <= 1;
				S2:			serial_out <= 0;
				S3:			serial_out <= 1;
				S4:			serial_out <= 0;
				S5:			serial_out <= 0;
				S6:			serial_out <= dirs[3];
				S7:			serial_out <= dirs[2];
				S8:			serial_out <= dirs[1];
				S9:			serial_out <= dirs[0];
				default:		serial_out <= 1;
			endcase
			
	always @(*)
		case(state)
			S0:	if(trans)	next <= S1;
					else			next <= S0;
			S1:					next <= S2;
			S2:					next <= S3;
			S3:					next <= S4;
			S4:					next <= S5;
			S5:					next <= S6;
			S6:					next <= S7;
			S7:					next <= S8;
			S8:					next <= S9;
			S9:					next <= S0;
			default:				next <= S0;
		endcase
		
endmodule


/*
 * Determines when the CPLD should be driving the data line, keeps the four bit counter
 * in sync, and determines when to latch the data from the shift register.  When the count
 * is below 18, the CPLD is sending the button status signal, so it allows the CPLD to 
 * drive the data line.  For the rest of the count, the data line is allowed to be driven
 * by the controller.  When the count hits 50, the first 16 bits of the controller status
 * are in the shift register, so the latch signal goes high to indicate that the Flip Flop
 * should latch onto the data.  When the count is about to reset, the count on the Four
 * Bit counter is externally reset to maintain synchronization.  Operates off the 500 kHz 
 * clock.
 */
module IOSelect (
	input clk,
	input reset,
	output reg [7:0] count,
	output reg count_reset,
	output reg transmit,
	output reg latch,
	output reg select
	);
	
	always @(posedge clk, negedge reset)
		if(!reset)	count <= 8'b0;	
		else			count <= count + 1;
		
	always @(posedge clk, negedge reset)
		if(!reset)							select <= 0;
		else if(count < 8'b00010010)	select <= 0;
		else 									select <= 1;
		
	always @(posedge clk, negedge reset)
		if(!reset)							count_reset <= 0;
		else if(count == 8'b11111111)	count_reset <= 1;
		else									count_reset <= 0;		
		
	always @(posedge clk, negedge reset)
		if(!reset)							latch <= 0;
		else if(count == 8'b00110010)	latch <= 1;
		else									latch <= 0;
	
	always @(posedge clk, negedge reset)
		if(!reset)							transmit <= 0;
		else if(count > 8'b01001101)	transmit <= 1;
		else									transmit <= 0;
		
endmodule


/*
 * Counter for the Logic High and Low signal generators.  Operates off
 * the 1 MHz clock.
 */
module TwoBitCounter (
	input clk,
	input reset,
	output reg [1:0] count
	);
	
	always @(posedge clk, negedge reset)
		if(!reset)	count <= 2'b0;
		else			count <= count + 1;
		
endmodule


/*
 * Makes sure that the CPLD doesn't drive the data line of the N64 when it
 * is responding to the status request.
 */
module Mux (
	input serial,
	input select,
	output reg out
	);
	
	always @(*)
		if(select)		out <= 1'bz;
		else 				out <= serial;
		
endmodule


/* 
 * Counts from 0 to 9 and used by the signal generator to know when 0's
 * and 1's should be sent to the controller.  An external reset input is
 * needed so that this counter can stay in sync with the IO Select module.  
 * Operates off the 250 kHz clock.
 */
module FourBitCounter (
	input clk,
	input reset,
	input ext_reset,
	output reg [3:0] count
	);
	
	always @(posedge clk, negedge reset, posedge ext_reset)
		if(!reset)						count <= 4'b0;
		else if(ext_reset)			count <= 4'b0;
		else if(count == 4'b1000)	count <= 4'b0;
		else								count <= count + 1;
		
endmodule


/* 
 *Continually generates the asynchronous 1 represenation that the N64 uses.
 */
module LogicHigh (
	input [1:0] count,
	output reg out
	);
	
	always @(*)
		case(count)
			2'b1: 	out = 1'b0;
			default:	out = 1'b1;
		endcase
endmodule 


/* 
 * Continually generates the asynchronous 0 representation that the N64 uses.
 */
module LogicLow (
	input [1:0] count,
	output reg out
	);
	
	always @(*)
		case(count)
			2'b00:	out = 1'b1;
			default:	out = 1'b0;
		endcase
endmodule
