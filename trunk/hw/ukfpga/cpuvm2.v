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
    input virq,			// ��������� ����������
    input evnt,			// ��������� ����������
    input halt,			// �����
    input aclo,			// ������ AC
    input dclo,			// ������ DC
    input ar,				// ����� ������
    input dmr,				// ������ �� ���
    input sack,			// ������������ ���
    input rply,			// ����� ��
    input sel,			// ��������� ������ (���������)
	 
    output dmgo,			// �������� ���
    output iako,			// �������� ����������
    output init,			// ����� ��
    output sync,			// ������������� ������
    output din,			// ������ ��
    output dout,			// ����� ��
    output wtbt,			// ������-����

    inout [15:0] ad		
    );
	 
	 //FSM ����������
	// f1 fetch;	   clock isn
   // c1 decode;
   //
   // s1 source1;	����� + sync
   // s2 source2;	!sync + din
   // s3 source3;	������
   // s4 source4;	!din
   //
   // d1 dest1;	����� + sync
   // d2 dest2;	!sync + �����
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
   // �����,������,	ea1	ea2		ea3			������			�������� ������
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
   parameter 	f1 = 5'b00001;
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
   parameter 	i1 = 5'b10110;
	
	wire [4:0] new_istate; //����� ���������
	reg [4:0] istate;     //������� ���������
	
	reg [15:0] r0,r1,r2,r3,r4,r5,r6;
	reg [15:0] pc;
	reg [15:0] psw;
	
	reg [15:0] spc; 	
	reg [15:0] spsw;
	
	wire [15:0] pc_mux;  //������������� ��� ���������� PC
	
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
	assign sp=r6;
	
	wire [2:0] ss_mode,ss_reg;
	wire [2:0] dd_mode,dd_reg;
	
	reg [15:0] ss_data; //��������� ������ S
	reg [15:0] dd_data; //��������� ������ D
	reg [15:0] e1_data; //��������� ������ ���������� -- ������� �����
	reg [15:0] e1_datah;//��������� ������ ���������� -- ������� �����
	
	wire [15:0] ss_data_mux; //������������� S
	wire [15:0] dd_data_mux; //������������� D
	wire [15:0] e1_result;   //������������� ����������
	wire [15:0] e1_resulth;	 //������������� ���������� (������� �����)
	
	reg [15:0] inst;
	reg [15:0] dd_ea;
	reg [15:0] ss_ea;
	
	wire trap;
	wire irq;
	wire oddpc;
	
	
	assign oddpc=pc[0];
	
//��������� ����
	assign new_istate= istate==f1?((trap||irq||oddpc)?t1:c1):
							 istate==c1?s1:
							 istate==s1?s2:
							 istate==s2?s3:
							 istate==s3?s4:
							 istate==s4?d1:
							 istate==d1?d2:
							 istate==d2?d3:
							 istate==d3?d4:
							 istate==d4?e1:
							 istate==e1?w1:
							 istate==w1?f1:
							 
							 //�����������
							 istate==o1?o2:
							 istate==o2?f1:
							 istate==o3?w1:							 
							 istate==p1?e1:							 
							 istate==t1?t2:
							 istate==t2?t3:
							 istate==t3?t4:
							 istate==t4?f1:
							 
							 //����������?
							 istate==i1?(irq?f1:i1):
							 
							 
							 istate;



endmodule
