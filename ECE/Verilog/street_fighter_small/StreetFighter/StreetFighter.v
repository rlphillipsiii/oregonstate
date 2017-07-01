`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:25:23 05/30/2014 
// Design Name: 
// Module Name:    StreetFighter 
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
module StreetFighter(
	input clk_ctrl,
	input clk_system,
   input ctrl_in,
	input ctrl_latch,
	input dir,
	input [3:0] move_select,
   input reset_n,
   output ctrl_out,
	
	// simulation debugging signals
	output reg override_en,
	output reg load_override,
	output reg [15:0] override_reg,
	output reg clk_sm,
	output reg [3:0] button_state,
	output reg [3:0] button_next,
	output reg [2:0] move,
	output reg [5:0] clk_listen,
	output reg [1:0] load_state,
	output reg [1:0] load_next,
	output reg [1:0] latch_count
   );

	// serial sequences for controller button presses
	parameter [15:0]
		but_down  = 16'b1111_1011_1111_1111,
		but_right = 16'b1111_1110_1111_1111,
		but_left  = 16'b1111_1101_1111_1111,
		but_y     = 16'b1011_1111_1111_1111,
		but_a     = 16'b1111_1111_0111_1111;
		
	// move state machine states
	parameter [3:0]
		down       = 4'b0000,
		down_right = 4'b0001,
		down_left  = 4'b0010,
		right      = 4'b0011,
		left       = 4'b0100,
		y_right    = 4'b0101,
		y_left     = 4'b0110,
		a_right    = 4'b0111,
		a_left     = 4'b1000,
		right_s    = 4'b1011,
		left_s     = 4'b1100,
		idle       = 4'b1001,
		complete   = 4'b1010;
		
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
	
	// move select logic
	always @(posedge clk_system, negedge reset_n)
		if (!reset_n)							  move <= none;
		else if (button_state == complete) move <= none;
		else if (override_en) 				  move <= move;
		else if (move_select == 4'b1110)   move <= dfp;
		else if (move_select == 4'b1101)   move <= dbk;
		else if (move_select == 4'b1011)   move <= bdfp;
		else if (move_select == 4'b0111)   move <= fdfp;
		
	// override enable control signal
	always @(posedge clk_system, negedge reset_n)
		if (!reset_n)							  override_en <= 1'b0;
		else if (button_state == complete) override_en <= 1'b0;
		else if (move != none)	  			  override_en <= 1'b1;

	// controller latch_counter
	always @(posedge clk_system, negedge reset_n)
		if (!reset_n)						 clk_sm <= 0;
		else if (latch_count == 2'b10) clk_sm <= 1;
		else if (completion_sync)		 clk_sm <= 1;
		else									 clk_sm <= 0;

	// controller latch_counter
	always @(posedge ctrl_latch, negedge reset_n)
		if (!reset_n) 						 latch_count <= 2'b00;
		else if (!override_en) 			 latch_count <= 2'b00;
		else if (latch_count == 2'b10) latch_count <= 2'b00;
		else 				 					 latch_count <= latch_count+1;
	
	// button state state machine
	always @(posedge clk_sm, negedge reset_n)
		if (!reset_n)	button_state <= idle;
		else				button_state <= button_next;
		
	// button state logic
	always @(*)
		case (button_state)
			idle       : if (move == dfp | move == dbk) 				 button_next = down;
							 else if (move == bdfp & dir == right_dir) button_next = left;
							 else if (move == bdfp & dir == left_dir)  button_next = right;
							 else if (move == fdfp & dir == right_dir) button_next = right_s;
							 else if (move == fdfp & dir == left_dir)	 button_next = left_s;
							 else													 button_next = idle;
							 
			down       : if ((move != dbk & dir == right_dir) | 
									(move == dbk & dir == left_dir)) button_next = down_right;
							 else										  		button_next = down_left;
							 
			down_right : if (move == bdfp & dir == left_dir) button_next = down;
							 else										 	 button_next = right;
							 
			down_left  : if (move == bdfp & dir == right_dir) button_next = down;
							 else										 	  button_next = left;
							 
			left       : if (move == fdfp)								 button_next = y_left;
							 else if (move == bdfp & dir == left_dir)  button_next = y_left;
							 else if (move == bdfp & dir == right_dir) button_next = down_left;
							 else if (move == dfp)  						 button_next = y_left;
							 else 												 button_next = a_left;
							 
			right      : if (move == fdfp)								 button_next = y_right;
							 else if (move == bdfp & dir == right_dir) button_next = y_right;
							 else if (move == bdfp & dir == left_dir ) button_next = down_right;
							 else if (move == dfp)  						 button_next = y_right;
							 else 												 button_next = a_right;
							 
			right_s    : button_next = down;
			
			left_s     : button_next = down;
			
			y_right    : button_next = complete;
			y_left     : button_next = complete;
			a_right    : button_next = complete;
			a_left     : button_next = complete;
			complete   : button_next = idle;
			
			default    : button_next = idle;
		endcase
		
	// completion sync pulse logic
	always @(posedge clk_system, negedge reset_n)
		if (!reset_n) 											completion_sync = 1'b0;
		else if (!clk_sm & button_state == complete)	completion_sync = 1'b1;
		else														completion_sync = 1'b0;
		
	// counter listening for the end of the 16 clock pulses
	always @(posedge clk_ctrl, posedge ctrl_latch)
		if (ctrl_latch)	clk_listen <= 5'b0;
		else					clk_listen <= clk_listen+1;
		
	// load override signal assignment
	always @(posedge clk_system, negedge reset_n)
		if(!reset_n)							load_override <= 1'b1;
		else if (load_state == load_reg) load_override <= 1'b0;
		else										load_override <= 1'b1;
		
	// load override signal state machine
	always @(posedge clk_system, negedge reset_n)
		if (!reset_n)	load_state <= wait_override;
		else				load_state <= load_next;
		
	// load override state machine next state logic
	always @(*)
		case (load_state)
			wait_override : if (override_en)	load_next <= wait_latch;
								 else					load_next <= wait_override;
								 
			wait_latch    : if (ctrl_latch)  load_next <= wait_clk;
								 else					load_next <= wait_latch;
								 
			wait_clk      : if (clk_listen == 5'b10000) load_next <= load_reg;
								 else								  load_next <= wait_clk;
								 
			load_reg      : if (override_en) load_next <= wait_latch;
								 else					load_next <= wait_override;
								 
			default       : load_next <= wait_override;
		endcase
			
	// override register controls
	always @(posedge clk_ctrl, negedge load_override)
		if (!load_override)	begin
			case (button_state)
				down       : override_reg <= but_down;
				down_right : override_reg <= but_down & but_right;
				down_left  : override_reg <= but_down & but_left;
				right      : override_reg <= but_right;
				right_s    : override_reg <= but_right;
				left       : override_reg <= but_left;
				left_s     : override_reg <= but_left;
				y_right    : override_reg <= but_right & but_y;
				y_left     : override_reg <= but_left & but_y;
				a_right    : override_reg <= but_right & but_a;
				a_left     : override_reg <= but_left & but_a;
				
				default    : override_reg <= 16'hFFFF;
			endcase
		end
		else 					 override_reg <= {override_reg[14:0], 1'b1};
			
	assign ctrl_out = (1'b0 == override_en) ? ctrl_in : override_reg[15];
endmodule
