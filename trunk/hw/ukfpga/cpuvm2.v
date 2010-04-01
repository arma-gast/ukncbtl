`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:43:31 03/30/2010 
// Design Name: 
// Module Name:    cpuvm2 
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
module cpuvm2(
    input clk,  			//
    input virq,			// векторное прерывание
    input evnt,			// таймерное прерывание
    input halt,			// пульт
    input aclo,			// авария AC
    input dclo,			// авария DC
    input ar,				// адрес принят
    input dmr,				// запрос на ПДП
    input sack,			// подверждение ПДП
    input rply,			// Ответ ВУ
    input sel,			// системная память (пультовая)
	 
    output dmgo,			// Разрешаю ПДП
    output iako,			// Разрешаю прерывания
    output init,			// сброс ВУ
    output sync,			// Синхронизация обмена
    output din,			// читаем ВУ
    output dout,			// пишем ВУ
    output wtbt,			// запись-байт

    inout [15:0] ad,		
	 
	 //отладка :)
	 
	 input dbgclk,
	 output [6:0] seg,
	 output dot,
	 output [3:0] an,	 	 
	 input [7:0] sw,
	 input [3:0] btn,
	 output [7:0] led
	 
    );
	 
	 //FSM процессора
	// f1  posedge(ad=addr,sync=1)
	// f2  posedge(ad=z, sync=0)   negedge(din=0)
	// f3  posedge(if rply==0 f4)  negedge(latch db)
	// f4  posedge(din=1...) move out..
	//
	
   // c1 decode;
   //
   // s1 source1;	адрес + sync
   // s2 source2;	!sync + din
   // s3 source3;	читаем
   // s4 source4;	!din
   //
   // d1 dest1;	адрес + sync
   // d2 dest2;	!sync + пишем
   // d3 dest3;	dout
   // d4 dest4;	!dout
   //
   // e1 execute;	clock pc, sp & reg+-
   // w1 writeback;	clock e1_result
   //
   // o1 pop pc	mem read
   // o2 pop psw	mem read
   // o3 pop reg	mem read
   //
   // p1 push sp	mem write
   //
   // t1 push psw	mem write
   // t2 push pc	mem write
   // t3 read pc	mem read
   // t4 read psw	mem read
   //
   // i1 interrupt wait
   //
   // minimum states/instructions = 3
   // maximum states/instructions = 12
   //
   // режим,символ,	ea1	ea2		ea3			данные			побочный эффект
   // 
   // 0		R			x		x			x				R					x
   // 1		(R)		R		x			x				M[R]				x
   // 2		(R)+		R		x			x				M[R]				R<-R+n
   // 3		@(R)+		R		M[R]		x				M[M[R]]			R<-R+2
   // 4		-(R)		R-2	x			x				M[R-2]			R<-R-n
   // 5		@-(R)		R-2	M[R-2]	x				M[M[R-2]]		R<-R-2
   // 6		X(R)		PC		M[PC]+R	x				M[M[PC]+R]		x
   // 7		@X(R)		PC		M[PC]+R	M[M[PC]+R]	M[M[M[PC]+R]]	x
   //
   // mode 0 -
   //  f1  pc++
   //  c1
   //  d1  dd_data = r
   //  e1  e1_result
   //             
   // mode 1 -
   //  f1  pc++
   //  c1
   //  d1  dd_ea = r
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //
   // mode 2 -
   //  f1  pc++
   //  c1
   //  d1  dd_ea = r, r++
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //
   // mode 3 -
   //  f1  pc++
   //  c1  dd_ea = r
   //  d1  dd_ea = bus_data, r++
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //
   // mode 4 -
   //  f1  pc++
   //  c1
   //  d1  dd_ea = r-x, r--
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //
   // mode 5 -
   //  f1  pc++
   //  c1
   //  d1  dd_ea = r-x, r--
   //  d2  dd_ea = bus_data
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //
   // mode 6 -
   //  f1  pc++
   //  c1
   //  d1  dd_ea = pc, pc++
   //  d2  dd_ea = bus_data + regs[dd_reg]
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //
   // mode 7 -
   //  f1  pc++
   //  c1
   //  d1  dd_ea = pc, pc++
   //  d2  dd_ea = bus_data + regs[dd_reg]
   //  d3  dd_ea = bus_data
   //  d4  dd_data = bus_data	optional
   //  e1  e1_result
   //

	parameter 	h1 = 5'b00000;
   parameter 	f1 = 5'h1;
	parameter 	f2 = 5'h2;
	parameter   f3 = 5'h3;
	parameter   f4 = 5'h4;
	
/*	
   parameter 	c1 = 5'b00010;
   parameter 	s1 = 5'b00011;
   parameter 	s2 = 5'b00100;
   parameter 	s3 = 5'b00101;
   parameter 	s4 = 5'b00110;
   parameter 	d1 = 5'b00111;
   parameter 	d2 = 5'b01000;
   parameter 	d3 = 5'b01001;
   parameter 	d4 = 5'b01010;
   parameter 	e1 = 5'b01011;
   parameter 	w1 = 5'b01100;
   parameter 	o1 = 5'b01101;
   parameter 	o2 = 5'b01110;
   parameter 	o3 = 5'b01111;
   parameter 	p1 = 5'b10001;
   parameter 	t1 = 5'b10010;
   parameter 	t2 = 5'b10011;
   parameter 	t3 = 5'b10100;
   parameter 	t4 = 5'b10101;
   parameter 	i1 = 5'b10110;*/
	
	wire [4:0] new_istate; //новое состояние
	reg [4:0] istate;     //текущее состояние
	
	reg [15:0] gpr[7:0];
	wire [15:0] pc;
	reg [15:0] psw;
	
	reg [15:0] spc; 	
	reg [15:0] spsw;
		
	wire [15:0] pc_mux;  //мультиплексор для следующего PC
	
	wire cc_n,cc_z,cc_v,cc_c;
	wire cc_halt,cc_p,cc_t;
	wire sp;
	
	assign cc_c=psw[0];
	assign cc_v=psw[1];
	assign cc_z=psw[2];
	assign cc_n=psw[3];
	assign cc_t=psw[4];
	assign cc_p=psw[7];
	assign cc_halt=psw[8];
	assign sp=gpr[6];
	assign pc=gpr[7];
	
	wire [2:0] ss_mode,ss_reg;
	wire [2:0] dd_mode,dd_reg;
	
	reg [15:0] ss_data; //результат циклов S
	reg [15:0] dd_data; //результат циклов D
	reg [15:0] e1_data; //результат циклов выполнения -- младшее слово
	reg [15:0] e1_datah;//результат циклов выполнения -- старшее слово
	
	wire [15:0] ss_data_mux; //мультиплексор S
	wire [15:0] dd_data_mux; //мультиплексор D
	wire [15:0] e1_result;   //мультиплексор результата
	wire [15:0] e1_resulth;	 //мультиплексор результата (старшее слово)
	
	reg [15:0] inst;
	reg [15:0] dd_ea;
	reg [15:0] ss_ea;
	
	wire trap;
	wire irq;
	wire oddpc;
	
	//отладка
	wire [15:0] debugdata0;
	wire [3:0]  debugdata1;
	
	assign oddpc=pc[0];
	
	wire [3:0] dbtn;
	
	debounce bnc0( .clk(clk), .insig(btn[0]), .outsig(dbtn[0]));
	debounce bnc1( .clk(clk), .insig(btn[1]), .outsig(dbtn[1]));
	debounce bnc2( .clk(clk), .insig(btn[2]), .outsig(dbtn[2]));
	debounce bnc3( .clk(clk), .insig(btn[3]), .outsig(dbtn[3]));
				
	
	
//следующий цикл
	assign new_istate= istate==f1?((trap||irq||oddpc)?h1:f2):
							 istate==f2?f3:
							 istate==f3?f4: //todo -- wait for rply==0
							 istate==f4?h1:
							 istate;
//qbus шина							 
	assign din=( (istate==f2)||(istate==f3))?1'h0:1'hz; 
	assign dout=1;
	assign sync=( (istate==f1))?1'h1:1'h0; 
	assign ad=(istate==f1)?gpr[7]:
				 16'hz;

	always @(posedge clk)
	begin
		gpr[0]=16'h1000;
		gpr[1]=16'h1001;
		gpr[2]=16'h1002;
		gpr[3]=16'h1003;
		gpr[4]=16'h1004;
		gpr[5]=16'h1005;		
		gpr[6]=16'h1006;
		//gpr[7]=16'h1007;
	end
	
	always @(posedge clk) //next time do on clock
	begin
		if((istate==h1)&&(btn[0]))
		begin
			istate<=f1;			
			gpr[7]<=(gpr[7]+2);
			//gpr[7][7:3]<=4'h0;
		end		
		else
			istate<=new_istate;
			
	end
	
	always @(negedge clk) //next time do on clock
		if(istate==f3)
			inst<=ad;
		

//отладка
	debug idebug(
			.clk(dbgclk),    
			.data0(debugdata0),
			.data1(debugdata1),
			.seg(seg),
			.dot(dot),
			.an(an));
	

/*
	выборка режима отладки-- переключателями 7-4 -- 
	выборка данных для индикации -- переключателями 2,1,0
	
	режимы -- 0= основные регистры
	          1= скрытые регистры -- spsw, spc(0=psw, 7=spc, 6=spsw)
				 2= эффективный адрес
				 3= текущая инструкция
				 4= текущее состояние
				 5= следующее состояние
				 6= шина данных
				 
*/
	wire [15:0] hidregmux;
	assign hidregmux=	(sw[2:0]==3'h0)?psw:
							(sw[2:0]==3'h6)?spsw:
							(sw[2:0]==3'h7)?spc:
							16'hfeed;
	
	assign debugdata0=(sw[7:4]==4'h0)?gpr[sw[2:0]]:
							(sw[7:4]==4'h1)?hidregmux:
							(sw[7:4]==4'h3)?inst:
							(sw[7:4]==4'h4)?(istate):
							(sw[7:4]==4'h5)?(new_istate):
							(sw[7:4]==4'h6)?(ad):
							16'hfeed;
								
	assign debugdata1=4'b1000;

/*
светодиоды
*/
	assign led[0]=sync;
	assign led[1]=din;
	assign led[2]=dout;

endmodule
