`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:24:43 07/18/2012
// Design Name:   Remote
// Module Name:   C:/Users/Robert/Documents/Porn/Summer 2012/N64/wired_remote/N64_wired/poll_test.v
// Project Name:  N64_wired
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

module poll_test;

	// Inputs
	reg Clk;
	reg Global_Reset;
	reg Float;

	// Outputs
	wire Len;
	wire Ldir;
	wire Ren;
	wire Rdir;

	// Bidirs
	wire Serial_IO;

	// Instantiate the Unit Under Test (UUT)
	Remote uut (
		.Clk(Clk), 
		.Global_Reset(Global_Reset), 
		.Float(Float), 
		.Len(Len), 
		.Ldir(Ldir), 
		.Ren(Ren), 
		.Rdir(Rdir), 
		.Serial_IO(Serial_IO)
	);

	always
		begin
			Clk = 1; #10;
			Clk = 0; #10;
		end
		
	initial begin
		// Initialize Inputs
		Global_Reset = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		  
		Global_Reset = 1;  #2000;
	end
      
endmodule

