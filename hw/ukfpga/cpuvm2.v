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

	reg [15:0] gpr[7:0];
	reg [15:0] psw;
	reg [15:0] spsw;
	reg [15:0] spc;
	
	reg [7:0] istate;
	wire [7:0] new_istate;

	/*
		vnutrennaja shina :
				i_rd, i_wr, i_byte, i_done.
				[15:0]i_datai,[15:0] i_datao
				[15:0]i_addr
		QBUS:
				~SYNC, ~RPLY, ~DIN, ~DOUT, ~WTBT, ~BSY
				[15:0]AD
	*/
	
	reg i_rd,i_wr,i_byte;
	wire i_done;
	reg [15:0] i_datai;
	wire [15:0] i_datao;
	wire [15:0] i_addr;
	
	
	parameter [4:0]	BS_IDLE=0,
							BS_R0=1,
							BS_R1=2,
							BS_W0=3,
							BS_W1=4,
							BS_DONE=5,
							BS_ERROR=6;

	parameter BS_TIMEOUT	= 63; 
	reg [4:0] bs_state;
	wire [4:0] bs_newstate;
	reg [5:0] bs_timeout;	
	
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
	
	
	
	//отладка
	wire [15:0] debugdata0;
	wire [3:0]  debugdata1;
	
	//assign oddpc=pc[0];
	
	wire [3:0] dbtn;
	
	debounce bnc0( .clk(dbgclk), .insig(btn[0]), .outsig(dbtn[0]));
	debounce bnc1( .clk(dbgclk), .insig(btn[1]), .outsig(dbtn[1]));
	debounce bnc2( .clk(dbgclk), .insig(btn[2]), .outsig(dbtn[2]));
	debounce bnc3( .clk(dbgclk), .insig(btn[3]), .outsig(dbtn[3]));

	wire reset;
	wire allowexec;
	assign reset=~btn[3]; //tolko dla otladki
	
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
	reg clco;
	
	always @(posedge clk)
		clco<=!clco;
	
	assign i_addr=gpr[7];
	assign i_datao=gpr[0];
		
	always @(posedge clco)
	begin
		if(!reset)
			begin
				gpr[0]<=16'h0;
				gpr[1]<=16'h0;
				gpr[2]<=16'h0;
				gpr[3]<=16'h0;
				gpr[4]<=16'h0;
				gpr[5]<=16'h0;
				gpr[6]<=16'h0;
				gpr[7]<=16'hE0C0;
				i_rd<=0;
				i_wr<=0;
				i_byte<=0;
			end
		else
			begin
				if(allowexec)
					begin
						if(btn[0])
						begin
							i_rd<=1;						
						end
					end
				else
				if(bs_state==BS_DONE)
					i_rd<=0;				
			end
	end

//qbus shina							 

	
	
	
	assign bs_newstate = bs_state==BS_IDLE?(
										(reset && i_rd && !i_wr)?BS_R0:
										(reset && !i_rd && i_wr)?BS_W0:
										BS_IDLE):
								bs_state==BS_R0?(
										(reset && i_rd && !i_wr)?BS_R1:
										BS_IDLE):
								bs_state==BS_R1?(
										(reset && i_rd && !i_wr && rply)?BS_R1:
										(reset && i_rd && !i_wr && !rply)?BS_DONE:
										BS_IDLE):
								bs_state==BS_W0?(
										(reset && i_wr && !i_rd)?BS_W1:
										BS_IDLE):
								bs_state==BS_W1?(
										(reset && i_wr && !i_rd && rply)?BS_W1:
										(reset && i_wr && !i_rd && rply)?BS_DONE:
										BS_IDLE):
								bs_state==BS_DONE?(
										(reset && (i_wr || i_rd))?BS_DONE:
										BS_IDLE):
										BS_IDLE;

	assign wtbt= (bs_state==BS_R0)?1'd0:
					 (bs_state==BS_W0)?1'd1:
					 (bs_state==BS_R1)?i_byte:
					 (bs_state==BS_W1)?i_byte:
					 1'h1;
	assign sync= (bs_state==BS_R1)?1'd0:
					 (bs_state==BS_W1)?1'd0:
					 (bs_state==BS_DONE)?1'd0:
					 1'd1;
	assign din=	 (bs_state==BS_R1)?1'd0:
					 (bs_state==BS_DONE)?!i_rd:
					 1'd1;
	assign dout= (bs_state==BS_W1)?1'd0:
					 (bs_state==BS_DONE)?!i_wr:
					 1'd1;

	wire [15:0] adoutmux;
	
	
	
	assign ad= (bs_state==BS_R0)?i_addr:16'hz;
	assign adoutmux= (bs_state==BS_R0)?i_addr:16'hz;
	
	//assign ad=( (bs_state==BS_R0) || (bs_state==BS_W1) || (bs_state==BS_W1) )?adoutmux:16'hz;
	
	
	assign allowexec= (bs_state==BS_IDLE)?1:0;
	
	always @(posedge clk)
	begin
		if(reset)
		begin
			bs_state<=bs_newstate;
			if(bs_state==BS_R1)
				i_datai<=ad;		
		end
	end
	

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
				 7= tekuschee sostojanie shiny
				 8= sledujusche sostojanie shiny
				 
*/
	wire [15:0] hidregmux;
	assign hidregmux=	(sw[2:0]==3'h0)?psw:
							(sw[2:0]==3'h6)?spsw:
							(sw[2:0]==3'h7)?spc:
							16'hfeed;
	
	assign debugdata0=(sw[7:4]==4'h0)?gpr[sw[2:0]]:
							(sw[7:4]==4'h1)?hidregmux:
							(sw[7:4]==4'h3)?i_datai:
							(sw[7:4]==4'h4)?(istate):
							(sw[7:4]==4'h5)?(new_istate):
							(sw[7:4]==4'h6)?(ad):
							(sw[7:4]==4'h7)?(bs_state):
							(sw[7:4]==4'h8)?(bs_newstate):
							(sw[7:4]==4'h9)?(i_addr):
							16'hfeed;
								
	assign debugdata1=4'b1000;

/*
светодиоды
*/
	assign led[0]=sync;
	assign led[1]=din;
	assign led[2]=dout;

endmodule
