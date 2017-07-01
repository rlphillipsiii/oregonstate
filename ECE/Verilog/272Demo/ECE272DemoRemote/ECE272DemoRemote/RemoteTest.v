`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:02:06 06/30/2011
// Design Name:   Remote
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoRemote/ECE272DemoRemote/RemoteTest.v
// Project Name:  ECE272DemoRemote
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Remote
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module RemoteTest;

	// Inputs
	reg Clk_In;
	reg Reset_In;

	// Outputs
	wire Latch_Out;

	// Instantiate the Unit Under Test (UUT)
	Remote uut (
		.Clk_In(Clk_In), 
		.Reset_In(Reset_In), 
		.Latch_Out(Latch_Out)
	);
	
	

	initial begin
		Reset_In = 0;
#5		Reset_In = 1;
#5		Reset_In = 1;
#5    Reset_In = 0;


	end
      
endmodule

