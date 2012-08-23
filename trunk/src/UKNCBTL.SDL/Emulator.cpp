// Emulator.cpp

#include "stdafx.h"
#include <stdio.h>
#include <Share.h>
#include "Emulator.h"
#include "emubase/Emubase.h"

//////////////////////////////////////////////////////////////////////


CMotherboard* g_pBoard = NULL;

BOOL g_okEmulatorInitialized = FALSE;
BOOL g_okEmulatorRunning = FALSE;

DWORD m_dwEmulatorUptime = 0;  // UKNC uptime, seconds, from turn on or reset, increments every 25 frames
long m_nUptimeFrameCount = 0;

const int KEYEVENT_QUEUE_SIZE = 16;
WORD m_EmulatorKeyQueue[KEYEVENT_QUEUE_SIZE];
int m_EmulatorKeyQueueTop = 0;
int m_EmulatorKeyQueueBottom = 0;
int m_EmulatorKeyQueueCount = 0;


//////////////////////////////////////////////////////////////////////

const LPCTSTR FILE_NAME_UKNC_ROM = _T("uknc_rom.bin");

BOOL Emulator_Init()
{
    ASSERT(g_pBoard == NULL);

    CProcessor::Init();

    g_pBoard = new CMotherboard();

    BYTE buffer[32768];
    size_t dwBytesRead;

    // Load ROM file
    memset(buffer, 0, 32768);
    FILE* fpRomFile = ::_tfsopen(FILE_NAME_UKNC_ROM, _T("rb"), _SH_DENYWR);
    if (fpRomFile == NULL)
    {
        AlertWarning(_T("Failed to load ROM file."));
        return false;
    }
    dwBytesRead = ::fread(buffer, 1, 32256, fpRomFile);
    ASSERT(dwBytesRead == 32256);
    ::fclose(fpRomFile);

    g_pBoard->LoadROM(buffer);

    g_pBoard->Reset();

    //if (m_okEmulatorSound)
    //{
    //    SoundGen_Initialize(Settings_GetSoundVolume());
    //    g_pBoard->SetSoundGenCallback(SoundGen_FeedDAC);
    //}

    m_nUptimeFrameCount = 0;
    m_dwEmulatorUptime = 0;

    //// Allocate memory for old RAM values
    //for (int i = 0; i < 3; i++)
    //{
    //    g_pEmulatorRam[i] = (BYTE*) ::malloc(65536);  memset(g_pEmulatorRam[i], 0, 65536);
    //    g_pEmulatorChangedRam[i] = (BYTE*) ::malloc(65536);  memset(g_pEmulatorChangedRam[i], 0, 65536);
    //}

    g_okEmulatorInitialized = TRUE;
    return TRUE;
}

void Emulator_Done()
{
    ASSERT(g_pBoard != NULL);

    CProcessor::Done();

    //g_pBoard->SetSoundGenCallback(NULL);
    //SoundGen_Finalize();

    g_pBoard->SetSerialCallbacks(NULL, NULL);
    //if (m_hEmulatorComPort != INVALID_HANDLE_VALUE)
    //{
    //    ::CloseHandle(m_hEmulatorComPort);
    //    m_hEmulatorComPort = INVALID_HANDLE_VALUE;
    //}

    delete g_pBoard;
    g_pBoard = NULL;

    //// Free memory used for old RAM values
    //for (int i = 0; i < 3; i++)
    //{
    //    ::free(g_pEmulatorRam[i]);
    //    ::free(g_pEmulatorChangedRam[i]);
    //}

    g_okEmulatorInitialized = FALSE;
}


void Emulator_Start()
{
    g_okEmulatorRunning = TRUE;

    //m_nFrameCount = 0;
    //m_nTickCount = 0;
}
void Emulator_Stop()
{
    g_okEmulatorRunning = FALSE;
    //m_wEmulatorCPUBreakpoint = 0177777;
    //m_wEmulatorPPUBreakpoint = 0177777;
}

void Emulator_Reset()
{
    ASSERT(g_pBoard != NULL);

    g_pBoard->Reset();

    //m_nUptimeFrameCount = 0;
    //m_dwEmulatorUptime = 0;
}

void Emulator_KeyboardEvent(BYTE keyscan, int pressed)
{
    if (m_EmulatorKeyQueueCount == KEYEVENT_QUEUE_SIZE) return;  // Full queue

    int ctrl = 0;  //STUB
    unsigned char keyflags = (pressed ? 128 : 0) | (ctrl ? 64 : 0);
    WORD keyevent = MAKEWORD(keyscan, keyflags);

    m_EmulatorKeyQueue[m_EmulatorKeyQueueTop] = keyevent;
    m_EmulatorKeyQueueTop++;
    if (m_EmulatorKeyQueueTop >= KEYEVENT_QUEUE_SIZE)
        m_EmulatorKeyQueueTop = 0;
    m_EmulatorKeyQueueCount++;
}

WORD Emulator_GetKeyEventFromQueue()
{
    if (m_EmulatorKeyQueueCount == 0) return 0;  // Empty queue

    WORD keyevent = m_EmulatorKeyQueue[m_EmulatorKeyQueueBottom];
    m_EmulatorKeyQueueBottom++;
    if (m_EmulatorKeyQueueBottom >= KEYEVENT_QUEUE_SIZE)
        m_EmulatorKeyQueueBottom = 0;
    m_EmulatorKeyQueueCount--;

    return keyevent;
}

void Emulator_ProcessKeyEvent()
{
    // Process next event in the keyboard queue
    WORD keyevent = Emulator_GetKeyEventFromQueue();
    if (keyevent != 0)
    {
        BOOL pressed = ((keyevent & 0x8000) != 0);
        BYTE bkscan = LOBYTE(keyevent);
        g_pBoard->KeyboardEvent(bkscan, pressed);
    }
}

int Emulator_SystemFrame()
{
    //SoundGen_SetVolume(Settings_GetSoundVolume());

    g_pBoard->SetCPUBreakpoint(0177777);
    g_pBoard->SetPPUBreakpoint(0177777);

    //ScreenView_ScanKeyboard();
    Emulator_ProcessKeyEvent();
    
    if (!g_pBoard->SystemFrame())
        return 0;

    //// Calculate frames per second
    //m_nFrameCount++;
    //DWORD dwCurrentTicks = GetTickCount();
    //long nTicksElapsed = dwCurrentTicks - m_dwTickCount;
    //if (nTicksElapsed >= 1200)
    //{
    //    double dFramesPerSecond = m_nFrameCount * 1000.0 / nTicksElapsed;
    //    double dSpeed = dFramesPerSecond / 25.0 * 100;
    //    TCHAR buffer[16];
    //    _stprintf(buffer, _T("%03.f%%"), dSpeed);
    //    MainWindow_SetStatusbarText(StatusbarPartFPS, buffer);

    //    BOOL floppyEngine = g_pBoard->IsFloppyEngineOn();
    //    MainWindow_SetStatusbarText(StatusbarPartFloppyEngine, floppyEngine ? _T("Motor") : NULL);

    //    m_nFrameCount = 0;
    //    m_dwTickCount = dwCurrentTicks;
    //}

    //// Calculate emulator uptime (25 frames per second)
    //m_nUptimeFrameCount++;
    //if (m_nUptimeFrameCount >= 25)
    //{
    //    m_dwEmulatorUptime++;
    //    m_nUptimeFrameCount = 0;

    //    int seconds = (int) (m_dwEmulatorUptime % 60);
    //    int minutes = (int) (m_dwEmulatorUptime / 60 % 60);
    //    int hours   = (int) (m_dwEmulatorUptime / 3600 % 60);

    //    TCHAR buffer[20];
    //    _stprintf(buffer, _T("Uptime: %02d:%02d:%02d"), hours, minutes, seconds);
    //    MainWindow_SetStatusbarText(StatusbarPartUptime, buffer);
    //}

    return 1;
}

void Emulator_PrepareScreenRGB32(void* pImageBits, const DWORD* colors)
{
    if (pImageBits == NULL) return;
    if (!g_okEmulatorInitialized) return;

    // Tag parsing loop
    BYTE cursorYRGB = 0;
    BOOL okCursorType = 0;
    BYTE cursorPos = 128;
    BOOL cursorOn = FALSE;
    BYTE cursorAddress = 0;  // Address of graphical cursor
    WORD address = 0000270;  // Tag sequence start address
    BOOL okTagSize = FALSE;  // Tag size: TRUE - 4-word, FALSE - 2-word (first tag is always 2-word)
    BOOL okTagType = FALSE;  // Type of 4-word tag: TRUE - set palette, FALSE - set params
    int scale = 1;           // Horizontal scale: 1, 2, 4, or 8
    DWORD palette = 0;       // Palette
	BYTE pbpgpr = 7;         // 3-bit Y-value modifier
    for (int yy = 0; yy < 307; yy++) {

        if (okTagSize) {  // 4-word tag
            WORD tag1 = g_pBoard->GetRAMWord(0, address);
            address += 2;
            WORD tag2 = g_pBoard->GetRAMWord(0, address);
            address += 2;

            if (okTagType)  // 4-word palette tag
            {
                palette = MAKELONG(tag1, tag2);
            }
            else  // 4-word params tag
            {
                scale = (tag2 >> 4) & 3;  // Bits 4-5 - new scale value
                //TODO: use Y-value modifier
                pbpgpr = tag2 & 7;  // Y-value modifier
                cursorYRGB = tag1 & 15;  // Cursor color
                okCursorType = ((tag1 & 16) != 0);  // TRUE - graphical cursor, FALSE - symbolic cursor
                ASSERT(okCursorType==0);  //DEBUG
                cursorPos = ((tag1 >> 8) >> scale) & 0x7f;  // Cursor position in the line
                //TODO: Use cursorAddress
                cursorAddress = (tag1 >> 5) & 7;
                scale = 1 << scale;

            }
        }

        WORD addressBits = g_pBoard->GetRAMWord(0, address);  // The word before the last word - is address of bits from all three memory planes
        address += 2;

        // Calculate size, type and address of the next tag
        WORD tagB = g_pBoard->GetRAMWord(0, address);  // Last word of the tag - is address and type of the next tag
        okTagSize = (tagB & 2) != 0;  // Bit 1 shows size of the next tag
        if (okTagSize)
        {
            address = tagB & ~7;
            okTagType = (tagB & 4) != 0;  // Bit 2 shows type of the next tag
        }
        else
            address = tagB & ~3;
        if ((tagB & 1) != 0)
            cursorOn = !cursorOn;

        // Draw bits into the bitmap, from line 20 to line 307
        if (yy >= 19 && yy <= 306)
        {
            // Loop thru bits from addressBits, planes 0,1,2
            // For each pixel:
            //   Get bit from planes 0,1,2 and make value
            //   Map value to palette; result is 4-bit value YRGB
            //   Translate value to 24-bit RGB
            //   Put value to m_bits; repeat using scale value

            int x = 0;
            int y = yy - 19;
            DWORD* pBits = ((DWORD*)pImageBits) + y * 640;
            for (int pos = 0; ; pos++)
            {
                // Get bit from planes 0,1,2
                BYTE src0 = g_pBoard->GetRAMByte(0, addressBits);
                BYTE src1 = g_pBoard->GetRAMByte(1, addressBits);
                BYTE src2 = g_pBoard->GetRAMByte(2, addressBits);
                // Loop through the bits of the byte
                for (int bit = 0; bit < 8; bit++)
                {
                    // Make 3-bit value from the bits
                    //BYTE value012 = (src0 & 1) | (src1 & 1) * 2 | (src2 & 1) * 4;
					BYTE value012;
					// Map value to palette; result is 4-bit value YRGB
                    BYTE valueYRGB;
                    if (cursorOn && (pos == cursorPos) && (!okCursorType || (okCursorType && bit == cursorAddress)))
                        valueYRGB = cursorYRGB;
                    else
					{
						value012 = (src0 & 1) | (src1 & 1) * 2 | (src2 & 1) * 4;
						valueYRGB = (BYTE) (palette >> (value012 * 4)) & 15;
					}
                    DWORD valueRGB = colors[valueYRGB];

                    // Put value to m_bits; repeat using scale value
                    for (int s = 0; s < scale; s++)
                        *pBits++ = valueRGB;
                    x += scale;

                    // Shift to the next bit
                    src0 = src0 >> 1;
                    src1 = src1 >> 1;
                    src2 = src2 >> 1;
                }
                if (x >= 640)
                    break;  // End of line
                addressBits++;  // Go to the next byte
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////
