`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date:    12:08:19 05/05/2010
// Design Name:
// Module Name:    ClockDividerStage
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
module ClockDivider(
	input clk_system,
	input reset_n,
	output clk_2,
	output clk_4,
	output clk_8,
    output clk_16
	);

	ClockDividerStage Divider_1 (
    .clk_system(clk_system),
    .reset_n(reset_n),
    .clk_out(clk_2)
    );

	ClockDividerStage Divider_2 (
    .clk_system(clk_2),
    .reset_n(reset_n),
    .clk_out(clk_4)
    );

	ClockDividerStage Divider_3 (
    .clk_system(clk_4),
    .reset_n(reset_n),
    .clk_out(clk_8)
    );
    
    ClockDividerStage Divider_4 (
    .clk_system(clk_8),
    .reset_n(reset_n),
    .clk_out(clk_16)
    );


endmodule

module ClockDividerStage(
    input clk_system,
    input reset_n,
    output reg clk_out
    );

	always @ (posedge clk_system, negedge reset_n) begin
		if(!reset_n) clk_out <= 0;
		else         clk_out <= ~clk_out;
    end
endmodule
