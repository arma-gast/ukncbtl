// DebugView.cpp

#include "stdafx.h"
#include "UKNCBTL.h"
#include "Views.h"
#include "Emulator.h"
#include "emubase\Emubase.h"


//////////////////////////////////////////////////////////////////////

// Colors
#define COLOR_RED   RGB(255,0,0)
#define COLOR_BLUE  RGB(0,0,255)


HWND g_hwndDebug = (HWND) INVALID_HANDLE_VALUE;  // Debug View window handle

BOOL m_okDebugProcessor = FALSE;  // TRUE - CPU, FALSE - PPU
WORD m_wDebugCpuR[9];  // Old register values - R0..R7, PSW
WORD m_wDebugPpuR[9];  // Old register values - R0..R7, PSW
BOOL m_okDebugCpuRChanged[9];  // Register change flags
BOOL m_okDebugPpuRChanged[9];  // Register change flags

void DoDrawDebugView(HDC hdc);
BOOL DebugView_OnKeyDown(WPARAM vkey, LPARAM lParam);
void DrawProcessor(HDC hdc, CProcessor* pProc, int x, int y, WORD* arrR, BOOL* arrRChanged);
void DrawDisassemble(HDC hdc, CProcessor* pProc, WORD previous, int x, int y);
void DrawMemoryForRegister(HDC hdc, int reg, CProcessor* pProc, int x, int y);
void DrawPorts(HDC hdc, BOOL okProcessor, CMemoryController* pMemCtl, int x, int y);
void DrawChannels(HDC hdc, int x, int y);


//////////////////////////////////////////////////////////////////////


void DebugView_RegisterClass()
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= DebugViewWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= g_hInst;
    wcex.hIcon			= NULL;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= CLASSNAME_DEBUGVIEW;
    wcex.hIconSm		= NULL;

    RegisterClassEx(&wcex);
}

void CreateDebugView(HWND hwndParent, int x, int y, int width, int height)
{
    ASSERT(hwndParent != NULL);

    g_hwndDebug = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            CLASSNAME_DEBUGVIEW, NULL,
            WS_CHILD,
            x, y, width, height,
            hwndParent, NULL, g_hInst, NULL);

    ShowWindow(g_hwndDebug, SW_SHOW);
    UpdateWindow(g_hwndDebug);
}

LRESULT CALLBACK DebugViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            DoDrawDebugView(hdc);  // Draw memory dump

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        SetFocus(hWnd);
        break;
    case WM_KEYDOWN:
        return (LRESULT) DebugView_OnKeyDown(wParam, lParam);
    case WM_DESTROY:
        g_hwndDebug = (HWND) INVALID_HANDLE_VALUE;  // We are closed! Bye-bye!..
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return (LRESULT)FALSE;
}

BOOL DebugView_OnKeyDown(WPARAM vkey, LPARAM lParam)
{
    switch (vkey)
    {
    case VK_SPACE:
        m_okDebugProcessor = ! m_okDebugProcessor;
        InvalidateRect(g_hwndDebug, NULL, TRUE);
        break;
    case VK_ESCAPE:
        ConsoleView_Activate();
        break;
    default:
        return TRUE;
    }
    return FALSE;
}


//////////////////////////////////////////////////////////////////////

// Update after Run or Step
void DebugView_OnUpdate()
{
    CProcessor* pCPU = g_pBoard->GetCPU();
    ASSERT(pCPU != NULL);

    // Get new register values and set change flags
    for (int r = 0; r < 8; r++) {
        WORD value = pCPU->GetReg(r);
        m_okDebugCpuRChanged[r] = (m_wDebugCpuR[r] != value);
        m_wDebugCpuR[r] = value;
    }
    WORD pswCPU = pCPU->GetPSW();
    m_okDebugCpuRChanged[8] = (m_wDebugCpuR[8] != pswCPU);
    m_wDebugCpuR[8] = pswCPU;

    CProcessor* pPPU = g_pBoard->GetPPU();
    ASSERT(pPPU != NULL);

    // Get new register values and set change flags
    for (int r = 0; r < 8; r++) {
        WORD value = pPPU->GetReg(r);
        m_okDebugPpuRChanged[r] = (m_wDebugPpuR[r] != value);
        m_wDebugPpuR[r] = value;
    }
    WORD pswPPU = pPPU->GetPSW();
    m_okDebugPpuRChanged[8] = (m_wDebugPpuR[8] != pswPPU);
    m_wDebugPpuR[8] = pswPPU;
}

void DebugView_SetCurrentProc(BOOL okCPU)
{
    m_okDebugProcessor = okCPU;
    InvalidateRect(g_hwndDebug, NULL, TRUE);
}


//////////////////////////////////////////////////////////////////////
// Draw functions

void DoDrawDebugView(HDC hdc)
{
    ASSERT(g_pBoard != NULL);

    // Create and select font
    HFONT hFont = CreateMonospacedFont();
    HGDIOBJ hOldFont = SelectObject(hdc, hFont);
    int cxChar, cyLine;  GetFontWidthAndHeight(hdc, &cxChar, &cyLine);
    COLORREF colorOld = SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
    COLORREF colorBkOld = SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

    CProcessor* pDebugPU = (m_okDebugProcessor) ? g_pBoard->GetCPU() : g_pBoard->GetPPU();
    ASSERT(pDebugPU != NULL);
    WORD* arrR = (m_okDebugProcessor) ? m_wDebugCpuR : m_wDebugPpuR;
    BOOL* arrRChanged = (m_okDebugProcessor) ? m_okDebugCpuRChanged : m_okDebugPpuRChanged;
    WORD prevPC = (m_okDebugProcessor) ? g_wEmulatorPrevCpuPC : g_wEmulatorPrevPpuPC;

    LPCTSTR sProcName = pDebugPU->GetName();

    TextOut(hdc, cxChar * 1, 2 * cyLine, sProcName, 3);
    DrawProcessor(hdc, pDebugPU, cxChar * 6, 2 * cyLine, arrR, arrRChanged);

    // Draw disasseble and stack for the current processor
    DrawDisassemble(hdc, pDebugPU, prevPC, 0, 18 * cyLine);
    DrawMemoryForRegister(hdc, 6, pDebugPU, 36 * cxChar, 1 * cyLine);
    CMemoryController* pDebugMemCtl = pDebugPU->GetMemoryController();
    DrawPorts(hdc, m_okDebugProcessor, pDebugMemCtl, 58 * cxChar, 1 * cyLine);

    DrawChannels(hdc, 58 * cxChar, 24 * cyLine);

    SetTextColor(hdc, colorOld);
    SetBkColor(hdc, colorBkOld);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void DrawRectangle(HDC hdc, int x1, int y1, int x2, int y2)
{
    HGDIOBJ hOldBrush = ::SelectObject(hdc, ::GetSysColorBrush(COLOR_BTNSHADOW));
    PatBlt(hdc, x1, y1, x2 - x1, 1, PATCOPY);
    PatBlt(hdc, x1, y1, 1, y2 - y1, PATCOPY);
    PatBlt(hdc, x1, y2, x2 - x1, 1, PATCOPY);
    PatBlt(hdc, x2, y1, 1, y2 - y1 + 1, PATCOPY);
    ::SelectObject(hdc, hOldBrush);
}

void DrawProcessor(HDC hdc, CProcessor* pProc, int x, int y, WORD* arrR, BOOL* arrRChanged)
{
    int cxChar, cyLine;  GetFontWidthAndHeight(hdc, &cxChar, &cyLine);
    COLORREF colorText = GetSysColor(COLOR_WINDOWTEXT);

    DrawRectangle(hdc, x - cxChar, y - 8, x + cxChar + 26 * cxChar, y + 8 + cyLine * 14);

    // Registers
    for (int r = 0; r < 8; r++) {
        ::SetTextColor(hdc, arrRChanged[r] ? COLOR_RED : colorText);

        LPCTSTR strRegName = REGISTER_NAME[r];
        TextOut(hdc, x, y + r * cyLine, strRegName, (int) wcslen(strRegName));

        WORD value = arrR[r]; //pProc->GetReg(r);
        DrawOctalValue(hdc, x + cxChar * 3, y + r * cyLine, value);
        DrawBinaryValue(hdc, x + cxChar * 10, y + r * cyLine, value);
    }
    ::SetTextColor(hdc, colorText);

    // CPC value
    TextOut(hdc, x, y + 8 * cyLine, _T("PC'"), 3);
    WORD cpc = pProc->GetCPC();
    DrawOctalValue(hdc, x + cxChar * 3, y + 8 * cyLine, cpc);
    DrawBinaryValue(hdc, x + cxChar * 10, y + 8 * cyLine, cpc);

    // PSW value
    ::SetTextColor(hdc, arrRChanged[8] ? COLOR_RED : colorText);
    TextOut(hdc, x, y + 10 * cyLine, _T("PS"), 2);
    WORD psw = arrR[8]; // pProc->GetPSW();
    DrawOctalValue(hdc, x + cxChar * 3, y + 10 * cyLine, psw);
    TextOut(hdc, x + cxChar * 10, y + 9 * cyLine, _T("       HP  TNZVC"), 16);
    DrawBinaryValue(hdc, x + cxChar * 10, y + 10 * cyLine, psw);

    ::SetTextColor(hdc, colorText);

    // CPSW value
    TextOut(hdc, x, y + 11 * cyLine, _T("PS'"), 3);
    WORD cpsw = pProc->GetCPSW();
    DrawOctalValue(hdc, x + cxChar * 3, y + 11 * cyLine, cpsw);
    DrawBinaryValue(hdc, x + cxChar * 10, y + 11 * cyLine, cpsw);

    // Processor mode - HALT or USER
    BOOL okHaltMode = pProc->IsHaltMode();
    TextOut(hdc, x, y + 13 * cyLine, okHaltMode ? _T("HALT") : _T("USER"), 4);

    // "Stopped" flag
    BOOL okStopped = pProc->IsStopped();
    if (okStopped)
        TextOut(hdc, x + 6 * cxChar, y + 11 * cyLine, _T("STOP"), 4);

}

void DrawDisassemble(HDC hdc, CProcessor* pProc, WORD previous, int x, int y)
{
    int cxChar, cyLine;  GetFontWidthAndHeight(hdc, &cxChar, &cyLine);
    COLORREF colorText = GetSysColor(COLOR_WINDOWTEXT);

    CMemoryController* pMemCtl = pProc->GetMemoryController();
    WORD current = pProc->GetPC();

    // ������ �� ������ ���������� � �����
    const int nWindowSize = 30;
    WORD memory[nWindowSize + 2];
    for (int idx = 0; idx < nWindowSize; idx++) {
        BOOL okValidAddress;
        memory[idx] = pMemCtl->GetWordView(
                current + idx * 2 - 10, pProc->IsHaltMode(), TRUE, &okValidAddress);
    }

    //TextOut(hdc, x, y, _T(" address  value   instruction"), 29);
    //y += cyLine;

    WORD address = current - 10;
    WORD disasmfrom = current;
    if (previous >= address && previous < current)
        disasmfrom = previous;

    int length = 0;
    for (int index = 0; index < nWindowSize; index++) {  // ������ ������
        DrawOctalValue(hdc, x + 5 * cxChar, y, address);  // Address
        // Value at the address
        WORD value = memory[index];
        DrawOctalValue(hdc, x + 13 * cxChar, y, value);
        // Current position
        if (address == current)
            TextOut(hdc, x + 1 * cxChar, y, _T("PC>>"), 4);
        else if (address == previous)
        {
            ::SetTextColor(hdc, COLOR_BLUE);
            TextOut(hdc, x + 1 * cxChar, y, _T("  > "), 4);
        }

        if (address >= disasmfrom && length == 0) {
            TCHAR strInstr[8];
            TCHAR strArg[32];
            length = DisassembleInstruction(memory + index, address, strInstr, strArg);
            if (index + length <= nWindowSize)
            {
                TextOut(hdc, x + 21 * cxChar, y, strInstr, (int) wcslen(strInstr));
                TextOut(hdc, x + 29 * cxChar, y, strArg, (int) wcslen(strArg));
            }
            ::SetTextColor(hdc, colorText);
        }
        if (length > 0) length--;

        address += 2;
        y += cyLine;
    }
}

void DrawMemoryForRegister(HDC hdc, int reg, CProcessor* pProc, int x, int y)
{
    int cxChar, cyLine;  GetFontWidthAndHeight(hdc, &cxChar, &cyLine);

    WORD current = pProc->GetReg(reg);
    BOOL okExec = (reg == 7);

    // ������ �� ������ ���������� � �����
    WORD memory[16];
    CMemoryController* pMemCtl = pProc->GetMemoryController();
    for (int idx = 0; idx < 16; idx++) {
        BOOL okValidAddress;
        memory[idx] = pMemCtl->GetWordView(
                current + idx * 2 - 16, pProc->IsHaltMode(), okExec, &okValidAddress);
    }

    WORD address = current - 16;
    for (int index = 0; index < 16; index++) {  // ������ ������
        // �����
        DrawOctalValue(hdc, x + 4 * cxChar, y, address);

        // �������� �� ������
        WORD value = memory[index];
        DrawOctalValue(hdc, x + 12 * cxChar, y, value);

        // ������� �������
        if (address == current) {
            TextOut(hdc, x + 2 * cxChar, y, _T(">>"), 2);
            TextOut(hdc, x, y, REGISTER_NAME[reg], 2);
        }

        address += 2;
        y += cyLine;
    }

}

void DrawPorts(HDC hdc, BOOL okProcessor, CMemoryController* pMemCtl, int x, int y)
{
    int cxChar, cyLine;  GetFontWidthAndHeight(hdc, &cxChar, &cyLine);

    TextOut(hdc, x, y, _T("portaddr value"), 14);

    if (okProcessor)  // CPU
    {
        WORD value176640 = pMemCtl->GetPortView(0176640);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 1 * cyLine, 0176640);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 1 * cyLine, value176640);
        WORD value176642 = pMemCtl->GetPortView(0176642);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 2 * cyLine, 0176642);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 2 * cyLine, value176642);

        //TODO
    }
    else  // PPU
    {
        WORD value177010 = pMemCtl->GetPortView(0177010);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 1 * cyLine, 0177010);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 1 * cyLine, value177010);
        WORD value177012 = pMemCtl->GetPortView(0177012);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 2 * cyLine, 0177012);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 2 * cyLine, value177012);
        WORD value177014 = pMemCtl->GetPortView(0177014);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 3 * cyLine, 0177014);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 3 * cyLine, value177014);
        WORD value177016 = pMemCtl->GetPortView(0177016);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 4 * cyLine, 0177016);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 4 * cyLine, value177016);
        WORD value177020 = pMemCtl->GetPortView(0177020);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 5 * cyLine, 0177020);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 5 * cyLine, value177020);
        WORD value177022 = pMemCtl->GetPortView(0177022);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 6 * cyLine, 0177022);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 6 * cyLine, value177022);
        WORD value177024 = pMemCtl->GetPortView(0177024);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 7 * cyLine, 0177024);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 7 * cyLine, value177024);
        WORD value177026 = pMemCtl->GetPortView(0177026);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 8 * cyLine, 0177026);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 8 * cyLine, value177026);
        WORD value177054 = pMemCtl->GetPortView(0177054);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 9 * cyLine, 0177054);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 9 * cyLine, value177054);
        WORD value177700 = pMemCtl->GetPortView(0177700);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 10 * cyLine, 0177700);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 10 * cyLine, value177700);
        WORD value177716 = pMemCtl->GetPortView(0177716);
        DrawOctalValue(hdc, x + 1 * cxChar, y + 11 * cyLine, 0177716);
        DrawOctalValue(hdc, x + 9 * cxChar, y + 11 * cyLine, value177716);
    }
}

void DrawChannels(HDC hdc, int x, int y)
{
    int cxChar, cyLine;  GetFontWidthAndHeight(hdc, &cxChar, &cyLine);

    CProcessor* pCPU = g_pBoard->GetCPU();
    CProcessor* pPPU = g_pBoard->GetPPU();
    CMemoryController* pCPUMemCtl = pCPU->GetMemoryController();
    CMemoryController* pPPUMemCtl = pPPU->GetMemoryController();

    TextOut(hdc, x, y, _T("Channels:"), 9);

    TCHAR bufData[7];
    TCHAR buffer[32];
	chan_stc tmpstc;

    
	tmpstc=g_pBoard->GetChannelStruct(0,0,0);
    
    PrintOctalValue(bufData, tmpstc.data);
	wsprintf(buffer, _T("PPU CH:0 RX D:%s RDY:%d IRQ:%d"), bufData + 3, tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 1 * cyLine, buffer, lstrlen(buffer));

    tmpstc=g_pBoard->GetChannelStruct(0,1,0);
    PrintOctalValue(bufData, tmpstc.data);
    wsprintf(buffer, _T("PPU CH:1 RX D:%s RDY:%d IRQ:%d"), bufData + 3, tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 2 * cyLine, buffer, lstrlen(buffer));

    tmpstc=g_pBoard->GetChannelStruct(0,2,0);
    PrintOctalValue(bufData, tmpstc.data);
    wsprintf(buffer, _T("PPU CH:2 RX D:%s RDY:%d IRQ:%d"), bufData + 3, tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 3 * cyLine, buffer, lstrlen(buffer));

	tmpstc=g_pBoard->GetChannelStruct(0,0,1);
    wsprintf(buffer, _T("PPU CH:0 TX       RDY:%d IRQ:%d"), tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 4 * cyLine, buffer, lstrlen(buffer));

	tmpstc=g_pBoard->GetChannelStruct(0,1,1);
    wsprintf(buffer, _T("PPU CH:1 TX       RDY:%d IRQ:%d"), tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 5 * cyLine, buffer, lstrlen(buffer));


    tmpstc=g_pBoard->GetChannelStruct(1,0,0);
	PrintOctalValue(bufData, tmpstc.data);
    wsprintf(buffer, _T("CPU CH:0 RX D:%s RDY:%d IRQ:%d"), bufData + 3, tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 6 * cyLine, buffer, lstrlen(buffer));

    tmpstc=g_pBoard->GetChannelStruct(1,1,0);
    PrintOctalValue(bufData, tmpstc.data);
    wsprintf(buffer, _T("CPU CH:1 RX D:%s RDY:%d IRQ:%d"), bufData + 3, tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 7 * cyLine, buffer, lstrlen(buffer));
	
	tmpstc=g_pBoard->GetChannelStruct(1,0,1);
    wsprintf(buffer, _T("CPU CH:0 TX       RDY:%d IRQ:%d"), tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 8 * cyLine, buffer, lstrlen(buffer));
	
	tmpstc=g_pBoard->GetChannelStruct(1,1,1);
    wsprintf(buffer, _T("CPU CH:1 TX       RDY:%d IRQ:%d"), tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 9 * cyLine, buffer, lstrlen(buffer));

	tmpstc=g_pBoard->GetChannelStruct(1,2,1);
    wsprintf(buffer, _T("CPU CH:2 TX       RDY:%d IRQ:%d"), tmpstc.ready, tmpstc.irq);
    TextOut(hdc, x, y + 10 * cyLine, buffer, lstrlen(buffer));


}


//////////////////////////////////////////////////////////////////////