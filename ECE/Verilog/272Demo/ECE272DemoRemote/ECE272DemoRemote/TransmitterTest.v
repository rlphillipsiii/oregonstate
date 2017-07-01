`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:01:34 07/07/2011
// Design Name:   transmitter
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoRemote/ECE272DemoRemote/TransmitterTest.v
// Project Name:  ECE272DemoRemote
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: transmitter
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module TransmitterTest;

	// Inputs
	reg [4:0] count;
	reg [4:0] data;

	// Outputs
	wire serial_data;

	// Instantiate the Unit Under Test (UUT)
	transmitter uut (
		.count(count), 
		.data(data), 
		.serial_data(serial_data)
	);

	initial begin
		// Initialize Inputs
		count = 0;
		data = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
      
endmodule

