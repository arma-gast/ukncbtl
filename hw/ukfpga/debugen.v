`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:08:29 03/29/2010 
// Design Name: 
// Module Name:    debugen 
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
module debugen(
    input clk,
    output [15:0] data0
    );
	
	reg [15:0] count;
	
	always@(posedge clk)
		count<=count+1;
		//count<=16'hfe82;
		
	assign data0=count;

endmodule
