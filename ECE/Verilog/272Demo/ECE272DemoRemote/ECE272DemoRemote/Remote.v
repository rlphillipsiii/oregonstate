`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Tekbots
// Engineer: Brandon Fry
// 
// Create Date:    14:50:39 06/29/2011 
// Design Name:    NES Wireless Remote
// Module Name:    ECE272DemoRemote 
// Project Name:    ECE272 Demo
// Target Devices:   XC9572 CPLD
// Tool versions: 
// Description:     Takes data from the NES remote and sends it over infrared in a form that is understood by the reciever
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Remote(
    input Clk_In,
	 input Reset_In,
	 input Controller_In,
    output Latch_Out,
	 output Clk_Out,
	 output Serial_Out
    );
	 
	 wire [4:0] count;
	 
	 assign Serial_Out = (count == 5'b11010)||(count == 5'b11011)||(count == 5'b11110)||(count == 5'b11111)||(~Controller_In);

four_bit_counter FBC (
    .clk(Clk_In), 
    .reset(Reset_In), 
    .count(count)
    );
	 
clock_state_decoder CSD (
    .count(count), 
    .clock(Clk_Out)
    );

latch_state_decoder LSD (
    .count(count), 
    .latch(Latch_Out)
    );
	 
//transmitter TST (
//    .count(count), 
//    .data(Controller_In), 
//    .serial_data(Serial_Out)
//    );

endmodule

module four_bit_counter(
    input clk, reset,
    output reg [4:0] count);

    always @ (posedge clk)
        if(!reset) count <= 5'b0;
        else count <= count + 1;
endmodule


module latch_state_decoder(
    input [4:0] count,
    output reg latch);

    always @(*)
        case(count)
        5'h0: latch = 1;
        default: latch = 0;
        endcase
endmodule

module clock_state_decoder(
    input [4:0] count,
    output reg clock);

    always @(*)
        case (count)
        5'h2: clock = 1;
        5'h4: clock = 1;
        5'h6: clock = 1;
        5'h8: clock = 1;
        5'ha: clock = 1;
        5'hC: clock = 1;
        5'hE: clock = 1;
		  5'h10: clock = 1;
		  5'h12: clock = 1;
		  5'h14: clock = 1;
		  5'h16: clock = 1;
		  5'h18: clock = 1;
		  5'h1a: clock = 1;
		  5'h1c: clock = 1;
		  5'h1e: clock = 1;
        default: clock = 0;
        endcase
endmodule

//module transmitter(
//    input [4:0] count, data,
//	 output reg serial_data);
//	 
//	 always @(*)
//			case (count)
//			5'h1a: serial_data = 1;
//			5'h1e: serial_data = 1;
//			default: serial_data = data;
//			endcase
//endmodule
	 
