`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:27:01 07/06/2011
// Design Name:   Shift_Register
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoReceiver/ECE272DemoReceiver/ShiftTest.v
// Project Name:  ECE272DemoReceiver
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Shift_Register
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module ShiftTest;

	// Inputs
	reg data_in;
	reg clk;
	reg reset;
	reg ShiftDump;

	// Outputs
	wire [11:0] q;
	wire [11:0] data_out;

	// Instantiate the Unit Under Test (UUT)
	Shift_Register uut (
		.data_in(data_in), 
		.clk(clk), 
		.reset(reset), 
		.ShiftDump(ShiftDump), 
		.q(q), 
		.data_out(data_out)
	);
	
	
	always
		begin
			clk = 1; #5; clk = 0; #5;
		end
		
	initial begin
		reset = 0; ShiftDump = 0; data_in = 0;
#10   reset = 1;
#10	reset = 0;
#10 	data_in = 1;
#10	data_in = 0;
#20	ShiftDump = 1;
#10   ShiftDump = 0;
#30 	ShiftDump = 1;
#10 	ShiftDump = 0;

	end
      
endmodule

