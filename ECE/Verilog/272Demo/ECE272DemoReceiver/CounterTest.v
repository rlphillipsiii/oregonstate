`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:43:27 07/06/2011
// Design Name:   five_bit_counter
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoReceiver/ECE272DemoReceiver/CounterTest.v
// Project Name:  ECE272DemoReceiver
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: five_bit_counter
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module CounterTest;

	// Inputs
	reg clk;
	reg reset;

	// Outputs
	wire complete;
	wire [4:0] count;

	// Instantiate the Unit Under Test (UUT)
	five_bit_counter uut (
		.clk(clk), 
		.reset(reset), 
		.complete(complete), 
		.count(count)
	);
	
	
	always
		begin
			clk = 1; #5; clk = 0; #5;
		end
  
	initial begin
	
		// Initialize Inputs
		reset = 0;
#10   reset = 1;
	end
      
endmodule

