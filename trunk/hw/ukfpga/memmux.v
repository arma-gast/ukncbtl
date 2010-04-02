`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:19:12 03/31/2010 
// Design Name: 
// Module Name:    memmux 
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
module memmux(
    input [15:0] ppu_addr,
    inout [15:0] ppu_data,
    input ppu_flcs,
    input ppu_ramcs,
    input ppu_memoe,
    input ppu_memrw,
    output ppu_rdy,
    input [15:0] cpu_addr,
    inout [15:0] cpu_data,
    input cpu_ramcs,
    input cpu_memoe,
    input cpu_memrw,
    output cpu_rdy,
    output [23:1] memaddr,
    inout [15:0] memdata,
    output memoe,
    output memrw,
    output memflcs,
    output memramcs,
    input clk
    );
	 
	 assign memoe=ppu_memoe;
	 assign memrw=ppu_memrw;
	 assign memflcs=ppu_flcs;
	 assign memramcs=ppu_ramcs;
	 assign memaddr[16:1]=ppu_addr[15:0];
	 assign memaddr[23:17]=7'd0;
	 
	 assign memdata=(memrw==0)?ppu_data:16'hz;	 
	 assign ppu_data=(!memflcs && memrw)?memdata:16'hz;
	 
	 assign ppu_rdy=clk; //always ready on pos edge



endmodule
