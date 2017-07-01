`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   16:54:24 07/06/2011
// Design Name:   Decoder
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoReceiver/ECE272DemoReceiver/DecoderTest.v
// Project Name:  ECE272DemoReceiver
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Decoder
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module DecoderTest;

	// Inputs
	reg [11:0] data_in;

	// Outputs
	wire [3:0] data_out;

	// Instantiate the Unit Under Test (UUT)
	Decoder uut (
		.data_in(data_in), 
		.data_out(data_out)
	);

	initial begin
		data_in = 12'b000011000000;
#10	data_in = 12'b000000110000;
		
	end
      
endmodule

