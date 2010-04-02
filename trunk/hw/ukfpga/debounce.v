`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:13:58 03/30/2010 
// Design Name: 
// Module Name:    debounce 
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
module debounce(
    input clk,
    input insig,
    output outsig
    );

 reg [14:0] clkdiv;
   reg 	      slowclk;
   reg [9:0]  hold;
   //reg 	      onetime;
   
   initial
     begin
	//onetime = 0;
	hold = 0;
	clkdiv = 0;
	slowclk = 0;
     end
   
   assign outsig = hold == 10'b1111111111 /*|| ~onetime*/;
		
   always @(posedge clk)
     begin
       clkdiv <= clkdiv + 15'b1;
       if (clkdiv == 0)
         slowclk <= ~slowclk;
     end

   always @(posedge slowclk)
     begin
			hold <= { hold[8:0], insig };
			//onetime <= 1;
     end

endmodule
