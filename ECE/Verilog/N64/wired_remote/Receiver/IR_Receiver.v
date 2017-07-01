`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:44:59 07/24/2012 
// Design Name: 
// Module Name:    IR_Receiver 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module IR_Receiver(
    input Serial_In,
	 input Global_Reset,
    input Clk,
    output Len,
    output Ldir,
    output Ren,
    output Rdir
    );

	wire [3:0] dirs, shift_out;
	wire serial_inv, latch, start;
	
	assign Len = dirs[3];
	assign Ldir = dirs[2];
	assign Ren = dirs[1];
	assign Rdir = dirs[0];

	Inverter inv (
		.in(Serial_In),
		.out(serial_inv)
		);

	ThreeBitCounter TBC (
		.clk(Clk),
		.reset(start),
		.count(),
		.latch(latch)
		);
		
	ShiftRegister SR (
		.serial(serial_inv),
		.reset(Global_Reset),
		.clk(Clk),
		.data(shift_out)
		);
		
	FlipFlop FF (
		.latch(latch),
		.reset(Global_Reset),
		.data(shift_out),
		.dirs(dirs)
		);
		
	IRListener IRL (
		.serial(shift_out[0]),
		.clk(Clk),
		.reset(Global_Reset),
		.start(start)
		);
		
endmodule


/*
 * The output from the IR transmit pin is active low, so every time the IR transmitter
 * sends a 1, it will appear as a 0 on the receiver side.  Therefore, an inverter is
 * needed to correct the discrepancy between the two signals.
 */
module Inverter (
	input in,
	output out
	);
	
	assign out = ~in;
	
endmodule


/*
 * Counts up to 3.  When the count reaches 3, the latch pin goes high because
 * the four direction bits have been loaded into the shift register, and the latch
 * pin going to high tells the Flip Flop to latch onto the state of the shift
 * register.
 */
module ThreeBitCounter (
	input clk,
	input reset,
	output reg latch,
	output reg [2:0] count
	);
	
	always @(posedge clk, posedge reset)
		if(reset)						count <= 3'b000;
		else if(count == 3'b010)	count <= 3'b000;
		else								count <= count + 1;
		
	always @(posedge clk, posedge reset)
		if(reset)						latch <= 0;
		else if(count == 3'b010)	latch <= 1;
		else								latch <= 0;
		
endmodule


/*
 * Continually samples the inverted serial line and shifts the bits to the left.
 */
module ShiftRegister (
	input serial,
	input reset,
	input clk,
	output reg [3:0] data
	);
	
	always @(posedge clk, negedge reset)
		if(!reset)	data <= 4'b0000;
		else			data <= {data[2:0], serial};
		
endmodule


/*
 * Latches onto the output of the shift register when the latch input goes high.
 */
module FlipFlop (
	input latch,
	input reset,
	input [3:0] data,
	output reg [3:0] dirs
	);
	
	always @(posedge latch, negedge reset)
		if(!reset)	dirs <= 4'b1111;
		else			dirs <= data;
		
endmodule


/*
 * Listens to the inverted serial input for the start sequence 101.  When the start
 * sequence is received, the rest of the circuit is activated and the next four bits
 * from the serial line are loaded into the shift register.
 */
module IRListener (
	input serial,
	input clk,
	input reset,
	output start
	);
	
	reg [2:0] state, next;
	
	parameter S0 = 4'b0000;
	parameter S1 = 4'b0001;
	parameter S2 = 4'b0010;
	parameter S3 = 4'b0011;
	parameter S4 = 4'b0100;
	parameter S5 = 4'b0101;
	parameter S6 = 4'b0110;
	parameter S7 = 4'b0111;
	parameter S8 = 4'b1000;
	
	always @(posedge clk, negedge reset)
		if(!reset)	state <= S0;
		else			state <= next;
		
	always @(*)
		case(state)
			S0:	if(serial)	next <= S1;
					else			next <= S0;
			S1:	if(~serial)	next <= S2;
					else			next <= S0;
			S2:	if(serial)	next <= S3;
					else			next <= S0;
			S3:	if(~serial)	next <= S4;
					else			next <= S0;
			S4:	if(~serial)	next <= S5;
					else			next <= S0;
			S5:					next <= S6;
			S6:					next <= S7;
			S7:					next <= S8;
			S8:					next <= S0;
			default:				next <= S0;
		endcase
		
	assign start = (state == S0) | (state == S1) | (state == S2) | (state == S3) | (state == S4);
	
endmodule
