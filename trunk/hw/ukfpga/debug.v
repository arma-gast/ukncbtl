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
	reg [3:0] digo;
	reg [6:0] sego;
	reg doto;
	reg [3:0] ano;
	
	always @(posedge clk)
	begin
		if(count==2'h0)
			begin
				digo<=data0[3:0];
				doto<=data1[0];				
				ano=4'h8;
			end
		else
		if(count==2'h1)
			begin
				digo<=data0[7:4];
				doto<=data1[1];				
				ano=4'h1;
			end
		else
		if(count==2'h2)
			begin
				digo<=data0[11:8];
				doto<=data1[2];				
				ano=4'h2;
			end
		else
		if(count==2'h3)
			begin
				digo<=data0[15:12];
				doto<=data1[3];				
				ano=4'h4;
			end
		//count<=count+1;
	end
	
	always @(negedge clk)
		count<=count+1;
	
	always @(posedge clk)
	begin
		
		if(digo==4'h0)
			sego<=7'h3f;
		else
		if(digo==4'h1)
			sego<=7'h06;
		else
		if(digo==4'h2)
			sego<=7'h5b;
		else
		if(digo==4'h3)
			sego<=7'h4f;
		else
		if(digo==4'h4)
			sego<=7'h66;
		else
		if(digo==4'h5)
			sego<=7'h6d;
		else
		if(digo==4'h6)
			sego<=7'h7d;
		else
		if(digo==4'h7)
			sego<=7'h07;
		else
		if(digo==4'h8)
			sego<=7'h7f;
		else
		if(digo==4'h9)
			sego<=7'h6f;
		else
		if(digo==4'ha)
			sego<=7'h77;
		else
		if(digo==4'hb)
			sego<=7'h7c;
		else
		if(digo==4'hc)
			sego<=7'h39;
		else
		if(digo==4'hd)
			sego<=7'h5e;
		else
		if(digo==4'he)
			sego<=7'h79;
		else
		if(digo==4'hf)
			sego<=7'h71;
			
	end
		
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
