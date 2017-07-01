`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:26:59 07/20/2012
// Design Name:   IOSelect
// Module Name:   C:/Users/Robert/Documents/Porn/Summer 2012/N64/wired_remote/N64_wired/SampleTest.v
// Project Name:  N64_wired
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: IOSelect
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module SampleTest;

	// Inputs
	reg Clock_2;
	reg Clock_4;
	reg Clock_8;
	reg reset;
	reg Serial_IO;
	
	// Outputs
	wire [6:0] count;
	wire count_reset;
	wire shift_clk;
	wire latch;
	wire select;
	wire [15:0] shift_out;
	wire [15:0] parallel_out;
	wire [3:0] dirs;
	
	// Instantiate the Unit Under Test (UUT)
	IOSelect uut (
		.clk(Clock_4), 
		.reset(reset), 
		.count(count), 
		.count_reset(count_reset), 
		.latch(latch), 
		.select(select)
		);

	Inverter inv (
		.clk(Clock_8),
		.reset(reset),
		.shift_clk(shift_clk)
		);
		
	ShiftRegister SR (
		.reset(reset),
		.clk(shift_clk),
		.serial_in(Serial_IO),
		.buffer(shift_out)
		);
		
	FlipFlop FF (
		.reset(reset),
		.latch(latch),
		.in(shift_out),
		.out(parallel_out)
		);
		
	Decoder Decode(
		.state(parallel_out),
		.dirs(dirs)
		);
		
	always
		begin
			Clock_2 = 1;	Clock_4 = 1;	Clock_8 = 1;	#5;
			Clock_2 = 0;											#5;
			Clock_2 = 1;	Clock_4 = 0;						#5;
			Clock_2 = 0;											#5;
			Clock_2 = 1;	Clock_4 = 1;	Clock_8 = 0;	#5;
			Clock_2 = 0;											#5;
			Clock_2 = 1; 	Clock_4 = 0;						#5;
			Clock_2 = 0;											#5;
		end

	
	initial begin
		// Initialize Inputs
		Clock_2 = 0;
		Clock_4 = 0;
		Clock_8 = 0;
		reset = 0;
		Serial_IO = 1;
		
		// Wait 120 ns for global reset to finish
		#120;
        
		reset = 1;
		/* logic 0 */
//		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		
		/* logic 1 */
//		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		
		/* 8'h01 => Get Button Status */
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		
		/* 32 bit response + 1 stop bit */
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		
		/* signal waiting period */
		#880;
		
		/* 8'h01 -> Get Button Status */
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		
		/* 32 bit response + 1 stop bit */
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		Serial_IO = 0;	#10;	Serial_IO = 0; #10		Serial_IO = 0; #10;	Serial_IO = 1;	#10;
		
		Serial_IO = 0;	#10;	Serial_IO = 1; #10		Serial_IO = 1; #10;	Serial_IO = 1;	#10;

	end
      
endmodule

