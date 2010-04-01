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

	parameter 	h1 = 8'h0;
   parameter 	f1 = 8'h1;
	parameter 	f2 = 8'h2;
	parameter   f3 = 8'h3;
	parameter   f4 = 8'h4;
	
	parameter	c1 = 8'he;
	
	parameter	d1	= 8'h11;
	parameter	d2 = 8'h12;
	parameter	d3 = 8'h13;
	parameter	d4 = 8'h14;
	parameter	d5 = 8'h15;
	parameter	d6 = 8'h16;
	parameter	d7 = 8'h17;
	parameter   d8 = 8'h18;
	parameter	d9 = 8'h19;

	parameter	s1	= 8'h21;
	parameter	s2 = 8'h22;
	parameter	s3 = 8'h23;
	parameter	s4 = 8'h24;
	parameter	s5 = 8'h25;
	parameter	s6 = 8'h26;
	parameter	s7 = 8'h27;
	parameter   s8 = 8'h28;
	parameter	s9 = 8'h29;

	
	wire [7:0] new_istate; //новое состояние
	reg [7:0] istate;     //текущее состояние
	
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
	wire opsize;

	reg [15:0] inst;
	reg [15:0] dd_ea;
	reg [15:0] ss_ea;


	assign ss_mode=inst[11:9];
	assign ss_reg=inst[8:6];
	assign dd_mode=inst[5:3];
	assign dd_reg=inst[2:0];
	assign opsize=inst[15];

   wire [3:0] 	inst_15_12;
   wire [6:0] 	inst_15_9;
   wire [9:0] 	inst_15_6;
   wire [5:0] 	inst_11_6;
   wire [2:0] 	inst_11_9;
   wire [5:0] 	inst_5_0;
	
 	
	wire need_ss_word;
	wire need_ss_byte;
	wire need_ss;
	wire need_dd_word;
	wire need_dd_byte;
	wire need_dd;
	
	reg [15:0] ss_data; //результат циклов S
	reg [15:0] dd_data; //результат циклов D
	reg [15:0] e1_data; //результат циклов выполнения -- младшее слово
	reg [15:0] e1_datah;//результат циклов выполнения -- старшее слово
	
	wire [15:0] ss_data_mux; //мультиплексор S
	wire [15:0] dd_data_mux; //мультиплексор D
	wire [15:0] e1_result;   //мультиплексор результата
	wire [15:0] e1_resulth;	 //мультиплексор результата (старшее слово)
	
	
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
				
	
	wire need_s2;
	wire need_s5;
	wire need_s6;
	
	assign need_s2 = (ss_mode>=3'o1);	
	assign need_s5 = (ss_mode>=3'o2);
	assign need_s6 = (ss_mode==3'o3) || (ss_mode==3'o5) || (ss_mode==3'o7);
	
	wire need_d2;
	wire need_d5;
	wire need_d6;
	
	assign need_d2 = (dd_mode>=3'o1);	
	assign need_d5 = (dd_mode>=3'o2);
	assign need_d6 = (dd_mode==3'o3) || (dd_mode==3'o5) || (dd_mode==3'o7);
	
//следующий цикл
	assign new_istate= istate==f1?((trap||irq||oddpc)?h1:f2):
							 istate==f2?f3:
							 istate==f3?f4: //todo -- wait for rply==0
							 istate==f4?c1:
							 istate==c1?(
								need_ss?s1:
								need_dd?d1:istate):


							 istate==s1?(need_s2?s2:
											(need_dd?d1:istate)):
							 istate==s2?s3:
							 istate==s3?s4:
							 istate==s4?(need_s5?s5:
											(need_dd?d1:istate)):
							 istate==s5?(need_s6?s6:
											(need_dd?d1:istate)):
							 istate==s6?s7:
							 istate==s7?s8:
							 istate==s8?s9:
							 istate==s9?(need_dd?d1:istate):

							 istate==d1?(need_d2?d2:istate):
							 istate==d2?d3:
							 istate==d3?d4:
							 istate==d4?(need_d5?d5:istate):
							 istate==d5?(need_d6?d6:istate):
							 istate==d6?d7:
							 istate==d7?d8:
							 istate==d8?d9:
							 //istate==d9?:							 
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
			if(gpr[7]==16'h0)
				gpr[7]<=16'hc0;
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


//декодируем инструкцию
  assign inst_15_12 = inst[15:12];
  assign inst_15_9  = inst[15:9];
  assign inst_15_6  = inst[15:6];
  assign inst_11_6  = inst[11:6];
  assign inst_11_9  = inst[11:9];
  assign inst_5_0   = inst[5:0];
  
	assign need_dd_word=
			(inst_15_6==10'o0001) || 		//jmp 	0001DD
			(inst_15_6==10'o0003) ||		//swab	0003dd
			(inst_15_12==0 && (inst_11_6 >=6'o40 && inst_11_6 <= 6'o63))||			//jsr - asl
			(inst_15_12==0 && inst_11_6 ==6'o67) ||				//sxt 0067dd
			(inst_15_12>=4'o01 && inst_15_12 <=4'o06) ||			//mov-add
			(inst_15_9 >=7'o070 && inst_15_9 <=7'o074) ||		//mul-xor
			(inst_15_6==10'o1064) || //mtps  1064SS !!!! note SS
			(inst_15_12==4'o16);		//sub
	assign need_dd_byte=
			(inst_15_12==4'o10 && (inst_11_6 >=6'o50 && inst_11_6 <=6'o64))|| //all .B inst
			(inst_15_6==4'o1067) || //mfps
			(inst_15_12>=4'o11 && inst_15_12<4'o16); 		//xxxxb
	assign need_dd= need_dd_word|need_dd_byte;
	
	assign need_ss_word=
			 (inst_15_12 >= 4'o01 && inst_15_12 <=4'o06) ||  //mov-add
			 (inst_15_12 == 4'o16); 		//sub
	assign need_ss_byte=
			 (inst_15_12 >=4'o11 && inst_15_12 <=4'o15); //movb-sub

	assign need_ss=need_ss_word|need_ss_byte;
			

		

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
				 2= эффективный адрес| инф о операндах (0=modes,1=regs, 2=moded, 3=regd, 4=ea s, 5=ea d)
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
