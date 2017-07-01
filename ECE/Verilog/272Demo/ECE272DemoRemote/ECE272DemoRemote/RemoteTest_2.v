`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   14:07:04 06/30/2011
// Design Name:   Remote
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoRemote/ECE272DemoRemote/RemoteTest_2.v
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

module RemoteTest_2;

	// Inputs
	reg Clk_In;
	reg Reset_In;
	reg Controller_In;

	// Outputs
	wire Latch_Out;
	wire Clk_Out;
	wire Serial_Out;

	// Instantiate the Unit Under Test (UUT)
	Remote uut (
		.Clk_In(Clk_In), 
		.Reset_In(Reset_In), 
		.Controller_In(Controller_In), 
		.Latch_Out(Latch_Out), 
		.Clk_Out(Clk_Out), 
		.Serial_Out(Serial_Out)
	);
	always
		begin
			Clk_In = 1; #5; Clk_In = 0; #5;
		end
		
	initial begin
		Controller_In = 0;
		Reset_In = 1;
#5		Reset_In = 1;
#5		Reset_In = 0;		
#5		Reset_In = 0;
#5		Reset_In = 1;
#5		Reset_In = 1;
#30	Controller_In = 0;
#5		Controller_In = 0;
#5		Controller_In = 1;
#30	Controller_In = 0;
#10	Controller_In = 1;

	end
      
endmodule

