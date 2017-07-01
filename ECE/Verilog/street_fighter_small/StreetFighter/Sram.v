`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:11:35 06/12/2014 
// Design Name: 
// Module Name:    Sram 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Sram(
    input clk,
    input reset_n,
    input read_addr,
    input set_en,
    output [15:0] data
    );

	// street fighter moves
	parameter [2:0]
		dfp  = 3'b000, // | \ --> Y
		dbk  = 3'b001, // | / <-- A
		bdfp = 3'b010, // <-- / | \ --> Y
		fdfp = 3'b011, // --> | \ --> Y
		none = 3'b100;
		
	// override register control signal state machines
	parameter [1:0]
		wait_override = 2'b00,
		wait_latch    = 2'b01,
		wait_clk      = 2'b10,
		load_reg      = 2'b11;
		
	parameter 
		left_dir  = 1'b1,
		right_dir = 1'b0;
		
	// serial sequences for controller button presses
	parameter [15:0]
		but_down  = 16'b1111_1011_1111_1111,
		but_right = 16'b1111_1110_1111_1111,
		but_left  = 16'b1111_1101_1111_1111,
		but_y     = 16'b1011_1111_1111_1111,
		but_a     = 16'b1111_1111_0111_1111,
		but_stop  = 16'hFFFF;
		
	reg [15:0] mem [63:0];
	
	always @(posedge clk, negedge reset_n)
		if (!reset_n) data <= 16'hFFFF;
		else 			  data <= mem[read_addr];
		
	always @(posedge clk, negedge reset_n)
		if (!reset_n) begin
			////////////// DFP ///////////////
			/* D DR R RY */
			mem[0] <= but_down;
			mem[1] <= but_down & but_right;
			mem[2] <= but_right;
			mem[3] <= but_right & but_y;
			mem[4] <= but_stop;
			
			/* D DL L LY */
			mem[5] <= but_down;
			mem[6] <= but_down & but_left;
			mem[7] <= but_left;
			mem[8] <= but_left & but_y;
			mem[9] <= but_stop;
			
			////////////// DBK ///////////////
			/* D DR R RA */
			mem[10] <= but_down;
			mem[11] <= but_down & but_right;
			mem[12] <= but_right;
			mem[13] <= but_right & but_a;
			mem[14] <= but_stop;
			
			/* D DL L LA */
			mem[15] <= but_down;
			mem[16] <= but_down & but_left;
			mem[17] <= but_left;
			mem[18] <= but_left & but_a;
			mem[19] <= but_stop;
			
			////////////// BDFP ///////////////
			/* L LD D DR R RY */
			mem[20] <= but_left;
			mem[21] <= but_left & but_down;
			mem[22] <= but_down;
			mem[23] <= but_down & but_right;
			mem[24] <= but_right;
			mem[25] <= but_right & but_y;
			mem[26] <= but_stop;
			
			/* R RD D DL L LY */
			mem[27] <= but_right;
			mem[28] <= but_right & but_down;
			mem[29] <= but_down;
			mem[30] <= but_down & but_left;
			mem[31] <= but_left;
			mem[32] <= but_left & but_y;
			mem[33] <= but_stop;
			
			////////////// FDFP ///////////////
			/* R D DR R RY */
			mem[34] <= but_right;
			mem[35] <= but_down;
			mem[36] <= but_down & but_right;
			mem[37] <= but_right;
			mem[38] <= but_right & but_y;
			mem[39] <= but_stop;
			
			/* L D DL L LY */
			mem[40] <= but_left;
			mem[41] <= but_down;
			mem[42] <= but_down & but_left;
			mem[43] <= but_left;
			mem[44] <= but_left & but_y;
			mem[45] <= but_stop;
		end
endmodule
