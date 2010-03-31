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
    output [15:0] data0,
	 output MemOE,
	 output [23:1] MemAdr,
	 input [15:0] MemDB,	 
	 input latch
    );
	
	reg [15:0] count;
	
	reg [1:0] state;
	wire [1:0] smux;
	
	reg MemOEo;
	reg [15:0] data0o;
	reg [23:1] MemAdro;
	assign MemAdr=MemAdro;
	assign data0=data0o;
	//assign data0[1:0]=state[1:0];
	assign MemOE=MemOEo;
	
	always@(posedge latch)
		//if(state==2'd3)
			count<=count+1;
	
	always@(posedge clk)
	begin
		//if(state==2'd3)
			//count<=count+1;
		if(state==2'd0)
			begin
				MemAdro[16:1]<=count[15:0];
				MemAdro[23:17]<=0;
				MemOEo<=1;				
			end
		else
		if(state==2'd1)
			begin
				MemOEo<=0;
			end
		else
		if(state==2'd2)
			begin
				MemOEo<=0;
				data0o<=MemDB;
			end
		if(state==2'd3)
			begin
				MemOEo<=1;
			end
			
	end
	
	always@(negedge clk)
		state<=smux;
	
	assign smux= state==2'd0?(latch==1?2'd1:2'd0):
					 state==2'd1?2'd2:
					 state==2'd2?2'd3:
					 state==2'd3?(latch==1?2'd3:2'd0):2'd0;
					 
		
	

endmodule
