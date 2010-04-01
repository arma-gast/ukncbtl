`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:49:08 03/29/2010 
// Design Name: 
// Module Name:    clkdiv 
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
module clkdiv(
    input clk,
    output clk6,
    output clk8,
    output clkps,
    output clkled,
	 output clkmem
    );
	 
	 reg [31:0] count6;
	 reg [31:0] count8;
	 reg [31:0] countled;
	 reg [31:0] countps;
	 reg [31:0] countmem;
	 reg clkpso;
	 reg clk6o;
	 reg clk8o;
	 reg clkledo;
	 reg clkmemo;
	 
	 always @(posedge clk)
	 begin
		count6<=count6+1;
		count8<=count8+1;
		countps<=countps+1;
		countled<=countled+1;
		countmem<=countmem+1;
		
		if(countmem==3)
		begin
			countmem<=0;
			clkmemo<=!clkmemo;
		end
		
		if(count6==8)
		begin
			count6<=0;
			clk6o<=!clk6o;
		end		
		if(count8==6)
		begin
			count8<=0;
			clk8o<=!clk8o;
		end
		
		if(countps==2500000)
		begin
			countps<=0;
			clkpso<=!clkpso;
		end		
		if(countled==25000)
		begin
			countled<=0;
			clkledo<=!clkledo;
		end		
		
	 end
	 
	 assign clkps=clkpso;
	 assign clk6=clk6o;
	 assign clk8=clk8o;
	 assign clkled=clkledo;
	 assign clkmem=clkmemo;

endmodule
