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


endmodule
