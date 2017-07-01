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
	input Clock,
	input Reset,
	output Clock_2,
	output Clock_4,
	output Clock_8
	);

	ClockDividerStage Divider_1 (
    .Clock(Clock),
    .Reset(Reset),
    .HalvedClock(Clock_2)
    );

	ClockDividerStage Divider_2 (
    .Clock(Clock_2),
    .Reset(Reset),
    .HalvedClock(Clock_4)
    );

	ClockDividerStage Divider_3 (
    .Clock(Clock_4),
    .Reset(Reset),
    .HalvedClock(Clock_8)
    );


endmodule

module ClockDividerStage(
    input Clock,
    input Reset,
	 output reg HalvedClock
    );

	always @ (posedge Clock, negedge Reset)
		if(!Reset) HalvedClock <= 0;
		else HalvedClock <= ~HalvedClock;
endmodule
