`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:49:32 03/30/2010 
// Design Name: 
// Module Name:    ppu-memctr 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
// all outputs are driven low, add pullup
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ppumemctr(
    input clk,
    input din,
    input dout,
    input sync,
    input wtbt,
    input halt,
    output rply,	 
	 inout [15:0] ad,
	 
	 output flashcs,
	 output memoe,
	 output memrw,
	 output [15:0] memaddr,
	 inout [15:0] memdata,
	 
	 input memrdy
	 
    );
	 
	 reg [1:0] dly;
	 reg [15:0] qaddr;
	 
	 
	  
	 always @(posedge clk)
	 begin
		if(sync)
			qaddr<=ad;
		dly<=dly+1;
	 end
		
	 assign ad=din?16'hz:memdata;
	 assign memaddr[14:0]=qaddr[14:0];
	 assign memaddr[15]=(din==0)?0:1'hz;
	 
	 //assign flashcs=(din)&&(dout); //for now
	 assign flashcs=(din)||(qaddr[15]==0);
	 
	 assign memoe=(din);
	 assign memrw=1;
	 
	 assign rply=((dly)||(din))==0?0:1'hz; 
	 
	 

endmodule
