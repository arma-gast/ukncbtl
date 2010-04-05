module ukfpga (
       input clk,

       output [6:0] seg,
       output dp,
       output [3:0] an,
       output [7:0] Led,
       input [7:0] sw,
       input [3:0] btn
       );



       assign seg=clk;
       assign dp=clk;
       assign Led=sw;
       assign an=btn;




endmodule
