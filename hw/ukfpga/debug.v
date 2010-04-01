`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:57:47 03/29/2010 
// Design Name: 
// Module Name:    debug 
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
module debug(
    input clk,    
	 input [15:0] data0,
	 input [3:0] data1,
	 
    output [6:0] seg,
    output dot,
	 output [3:0] an
    );

	reg [1:0] count;
	wire [3:0] digo;
	wire [6:0] sego;
	wire doto;
	wire [3:0] ano;
	
	
	always @(posedge clk)
			count<=count+1;
			
	assign digo=(count==2'h0)?data0[3:0]:
					(count==2'h1)?data0[7:4]:
					(count==2'h2)?data0[11:8]:
					data0[15:12];
					
	assign doto=(count==2'h0)?data1[0]:
					(count==2'h1)?data1[1]:
					(count==2'h2)?data1[2]:
					data1[3];

	assign ano=(count==2'h0)?4'h1:
					(count==2'h1)?4'h2:
					(count==2'h2)?4'h4:
					4'h8;

		
	assign sego=(digo==4'h0)?7'h3f:
					(digo==4'h1)?7'h06:
					(digo==4'h2)?7'h5b:
					(digo==4'h3)?7'h4f:
					(digo==4'h4)?7'h66:
					(digo==4'h5)?7'h6d:
					(digo==4'h6)?7'h7d:
					(digo==4'h7)?7'h07:
					(digo==4'h8)?7'h7f:
					(digo==4'h9)?7'h6f:
					(digo==4'ha)?7'h77:
					(digo==4'hb)?7'h7c:
					(digo==4'hc)?7'h39:
					(digo==4'hd)?7'h5e:
					(digo==4'he)?7'h79:
					7'h71;
			
	assign dot=!doto;
	assign seg[0]=!sego[0];
	assign seg[1]=!sego[1];
	assign seg[2]=!sego[2];
	assign seg[3]=!sego[3];
	assign seg[4]=!sego[4];
	assign seg[5]=!sego[5];
	assign seg[6]=!sego[6];
	assign an[0]=!ano[0];
	assign an[1]=!ano[1];
	assign an[2]=!ano[2];
	assign an[3]=!ano[3];

endmodule
