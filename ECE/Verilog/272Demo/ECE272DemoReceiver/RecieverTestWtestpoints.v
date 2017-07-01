`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:14:30 07/07/2011
// Design Name:   Receiver
// Module Name:   T:/tekbots/tekbots-guille/students/fryb/ECE272Demo/ECE272DemoReceiver/ECE272DemoReceiver/RecieverTestWtestpoints.v
// Project Name:  ECE272DemoReceiver
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Receiver
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module RecieverTestWtestpoints;

	// Inputs
	reg Serial_In;
	reg Reset_In;
	reg Clk_In;

	// Outputs
	wire Len;
	wire Ldir;
	wire Ren;
	wire Rdir;

	wire [11:0] decoder;
	wire transfer;
	wire clk;
	
	// Instantiate the Unit Under Test (UUT)
	Receiver UUT (
    .Serial_In(Serial_In), 
    .Reset_In(Reset_In), 
    .Clk_In(Clk_In), 
	 .decoder_in(decoder),
	 .transfer_enable(transfer),
	 .internal_clk(clk),
    .Len(Len), 
    .Ldir(Ldir), 
    .Ren(Ren), 
    .Rdir(Rdir)
	);

		always
		begin
		Clk_In = 1; #5; Clk_In = 0; #5;
		end

	initial begin
		// Initialize Inputs
		Serial_In = 0; Reset_In = 0;
#20	Reset_In = 0;
#20	Reset_In = 1;
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#100   Serial_In = 1;
#20	Serial_In = 0;
#140  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#100   Serial_In = 1;
#20	Serial_In = 0;
#140  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#100   Serial_In = 1;
#20	Serial_In = 0;
#140  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#100   Serial_In = 1;
#20	Serial_In = 0;
#140  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#100   Serial_In = 1;
#40	Serial_In = 0;
#100  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#100   Serial_In = 1;
#40	Serial_In = 0;
#100  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#80   Serial_In = 1;
#60	Serial_In = 0;
#100  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#80   Serial_In = 1;
#60	Serial_In = 0;
#100  Serial_In = 1;  //Start Bit
#20   Serial_In = 0;  //Start Bit
#20   Serial_In = 1;  //Start Bit
#20   Serial_In = 0;
#80   Serial_In = 1;
#60	Serial_In = 0;


	end
      
endmodule

