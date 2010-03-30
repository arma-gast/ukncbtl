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

    inout [15:0] ad		
    );


endmodule
