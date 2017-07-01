`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Tekbots
// Engineer: Brandon Fry & Robert Phillips III 
// 
// Create Date:    16:53:47 07/01/2011 
// Design Name: 	 ECE272 IR Receiver
// Module Name:    Receiver 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: IR Receiver boiiii...swerve
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Receiver(
    input Serial_In,
    input Reset_In,
    input Clk_In,
    output Len,
    output Ldir,
    output Ren,
    output Rdir,
	 output N64_Len,
	 output N64_Ldir,
	 output N64_Ren,
	 output N64_Rdir
    );
	
	 wire [3:0] Decoder_Out_Wire;
	 wire [11:0] Shift_Decoder;
	 wire Internal_Clk, Count_Reset, Count_Shift, DB_Shift, Serial_Invert,
				Left_Dir, Left_En, Right_Dir, Right_En;
	 
	 assign N64_Len = Left_En;
	 assign N64_Ldir = Left_Dir;
	 assign N64_Ren = Right_En;
	 assign N64_Rdir = Right_Dir;
	 
	 assign Len = Decoder_Out_Wire[3];
	 assign Ldir = Decoder_Out_Wire[2];
	 assign Ren = Decoder_Out_Wire[1];
	 assign Rdir = Decoder_Out_Wire[0];

IR_Receiver N64Receiver (
	.Serial_In(Serial_In),
	.Global_Reset(Reset_In),
	.Clk(Clk_In),
   .Len(Left_En),
   .Ldir(Left_Dir),
   .Ren(Right_En),
   .Rdir(Right_Dir)
	);
	
Inverter inv (
	 .in(Serial_In),
	 .out(Serial_Invert)
	 );
	 
ClockDivider CD (
    .Clock(Clk_In), 
    .Reset(Reset_In), 
    .Clock_2(Internal_Clk)
    );

five_bit_counter FBC (
    .clk(Internal_Clk), 
    .reset(Count_Reset),
	 .complete(Count_Shift),
    .count()
    );

Input_State_Machine instance_name (
    .clk(Internal_Clk), 
    .reset(Reset_In), 
    .data_in(Serial_Invert), 
    .count_reset(Count_Reset)
    );
	 
Data_Buffer DB (
    .data_in(Serial_Invert), 
    .transfer(Count_Reset), 
    .data_out(DB_Shift)
    );
	 
Shift_Register SR (
    .data_in(DB_Shift), 
    .clk(Internal_Clk), 
    .reset(Reset_In), 
    .ShiftDump(Count_Shift), 
    .q(), 
    .data_out(Shift_Decoder)
    );
	 
Decoder_12_4 DCDR (
    .data_in(Shift_Decoder), 
    .data_out(Decoder_Out_Wire)
    );
endmodule

module Input_State_Machine(input  clk,									
									input  reset,
									input  data_in,
									output count_reset);
									
	 reg [3:0] state, nextstate;
	 
	 parameter S0 = 4'b0000;
	 parameter S1 = 4'b0001;
	 parameter S2 = 4'b0010;
	 parameter S3 = 4'b0011;
	 parameter S4 = 4'b0100;
	 parameter S5 = 4'b0101;
	 parameter S6 = 4'b0110;
	 parameter S7 = 4'b0111;
	 parameter S8 = 4'b1000;
	 parameter S9 = 4'b1001;
	 parameter S10 = 4'b1010;
	 parameter S11 = 4'b1011;
	 parameter S12 = 4'b1100;
	 parameter S13 = 4'b1101;
	 parameter S14 = 4'b1110;
	 parameter S15 = 4'b1111;
	 
	 
	 always @(posedge clk, negedge reset)
		  if(!reset) state <= S0;
		  else		state <= nextstate;
		  
	 always @(*)
		case(state)
			S0:	if(data_in)   nextstate = S1;
					else			  nextstate = S0;
			S1:	if(~data_in)  nextstate = S2;
					else			  nextstate = S0;
			S2:	if(data_in)	  nextstate = S3;
					else			  nextstate = S0;
			S3: 					  nextstate = S4;
			S4:					  nextstate = S5;
			S5:					  nextstate = S6;
			S6:					  nextstate = S7;
			S7:					  nextstate = S8;
			S8:					  nextstate = S9;
			S9:					  nextstate = S10;
			S10:					  nextstate = S11;
			S11:					  nextstate = S12;
			S12:					  nextstate = S13;
			S13:					  nextstate = S14;
			S14:					  nextstate = S15;
			S15:					  nextstate = S0;
		endcase
	
	 assign count_reset = (state == S0)||(state == S1)||(state == S2);
endmodule

module Data_Buffer(input data_in,
						 input transfer,
						 output reg data_out);
	 always @(*)
		if(~transfer)	data_out <= data_in;
		else				data_out <= 0;
endmodule
			

module five_bit_counter(
    input clk, reset,
	 output reg complete,
    output reg [3:0] count);
	 
    always @ (posedge clk, posedge reset)
        if(reset) 							 count <= 4'b0;
		  else if(count == 5'b1100)		 count <= 4'b0;
        else 									 count <= count + 1;
		  
	 always @(*)
		if(count == 5'b1100)  complete <= 1;
		else						  complete <= 0;
		
endmodule

module Shift_Register(input data_in,
							 input clk,
							 input reset,
							 input ShiftDump,
							 output reg [11:0] q,
							 output reg [11:0] data_out);
							 
	 always @ (posedge clk, negedge reset)
		if(!reset)					q <= 0;
		else if(~ShiftDump) 		q <= {q[10:0], data_in};
		
	 always @ (posedge ShiftDump, negedge reset)
		if(!reset)		data_out <= 0;
		else				data_out <= q;
		
endmodule

module Decoder_12_4(input [11:0] data_in, 
						  output reg [3:0] data_out);
	 
	 always @(*)
		case(data_in)       
			/* B Y Sel Start_U D L R_A X LB RB */ /* Le Ldir Re Rdir */
			12'b0000_1000_0000:   data_out = 4'b0101;  //forward
			12'b0000_0100_0000:   data_out = 4'b0000;  //reverse
			12'b0000_0001_0000:   data_out = 4'b0001;  //spin right
			12'b0000_0010_0000:   data_out = 4'b0100;  //spin left
			12'b0000_1010_0000:   data_out = 4'b0110;  //veer left
			12'b0000_1001_0000:   data_out = 4'b1001;  //veer right
			12'b0000_0110_0000:   data_out = 4'b1000;  //back veer left
			12'b0000_0101_0000:   data_out = 4'b0010;  //back veer right
			default:	 data_out = 4'b1111;  //Stop
		endcase

endmodule
 