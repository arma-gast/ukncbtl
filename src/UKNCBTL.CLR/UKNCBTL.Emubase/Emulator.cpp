
#include "stdafx.h"
#include "Emulator.h"
#include "emubase\Emubase.h"

using namespace UKNCBTL;

Emulator::Emulator(void) :
    m_pBoard(0), m_okRunning(false),
    m_wCPUBreakpoint(0177777), m_wPPUBreakpoint(0177777)
{
    CProcessor::Init();

    m_pBoard = new CMotherboard();

    m_pBoard->Reset();

    m_nUptimeFrameCount = 0;
    m_dwEmulatorUptime = 0;
}

Emulator::~Emulator()
{
    delete m_pBoard;  m_pBoard = 0;

    CProcessor::Done();
}

void Emulator::SetCPUBreakpoint(System::UInt16 address)
{
    m_wCPUBreakpoint = address;
}
void Emulator::SetPPUBreakpoint(System::UInt16 address)
{
    m_wPPUBreakpoint = address;
}

float Emulator::GetUptime()
{
    return (float)m_dwEmulatorUptime + (float)m_nUptimeFrameCount / 25.0f;
}

void Emulator::Start()
{
    m_okRunning = true;
}

void Emulator::Stop()
{
    m_okRunning = false;
    m_wCPUBreakpoint = 0177777;
    m_wPPUBreakpoint = 0177777;
}

void Emulator::Reset()
{
    m_pBoard->Reset();

    m_nUptimeFrameCount = 0;
    m_dwEmulatorUptime = 0;
}

bool Emulator::SystemFrame()
{
    m_pBoard->SetCPUBreakpoint(m_wCPUBreakpoint);
    m_pBoard->SetPPUBreakpoint(m_wPPUBreakpoint);

    //TODO: ScreenView_ScanKeyboard();
    
    if (!m_pBoard->SystemFrame())
        return false;

    m_nUptimeFrameCount++;
    if (m_nUptimeFrameCount >= 25)
    {
        m_dwEmulatorUptime++;
        m_nUptimeFrameCount = 0;
    }

    return true;
}

bool Emulator::LoadROM(System::String ^sFilePath)
{
    //TODO: Very dirty, rewrite it
    array<unsigned char>^ bytes = System::IO::File::ReadAllBytes(sFilePath);
    cli::pin_ptr<unsigned char> pBytes = &bytes[0];

    m_pBoard->LoadROM(pBytes);

    return true; //STUB
}

//bool Emulator::LoadROMCartridge(int slot, System::String ^sFilePath)
//{
//    return false; //TODO
//}


void Emulator_PrepareScreenRGB32(CMotherboard* pBoard, void* pScan0, int nStride, const DWORD* colors)
{
    if (pBoard == NULL) return;
    if (pScan0 == NULL) return;

    // Tag parsing loop
    BYTE cursorYRGB;
    BOOL okCursorType;
    BYTE cursorPos = 128;
    BOOL cursorOn = FALSE;
    BYTE cursorAddress;      // Address of graphical cursor
    WORD address = 0000270;  // Tag sequence start address
    BOOL okTagSize = FALSE;  // Tag size: TRUE - 4-word, FALSE - 2-word (first tag is always 2-word)
    BOOL okTagType = FALSE;  // Type of 4-word tag: TRUE - set palette, FALSE - set params
    int scale = 1;           // Horizontal scale: 1, 2, 4, or 8
    DWORD palette = 0;       // Palette
    DWORD palettecurrent[8];  memset(palettecurrent, 0, sizeof(palettecurrent)); // Current palette; update each time we change the "palette" variable
	BYTE pbpgpr = 7;         // 3-bit Y-value modifier
    BYTE* pScan = (BYTE*) pScan0;
    for (int yy = 0; yy < 307; yy++)
    {
        if (okTagSize) {  // 4-word tag
            WORD tag1 = pBoard->GetRAMWord(0, address);
            address += 2;
            WORD tag2 = pBoard->GetRAMWord(0, address);
            address += 2;

            if (okTagType)  // 4-word palette tag
            {
                palette = MAKELONG(tag1, tag2);
                for (BYTE c = 0; c < 8; c++)  // Update palettecurrent
                {
                    BYTE valueYRGB = (BYTE) (palette >> (c << 2)) & 15;
                    palettecurrent[c] = colors[valueYRGB];
                }
            }
            else  // 4-word params tag
            {
                scale = (tag2 >> 4) & 3;  // Bits 4-5 - new scale value
                pbpgpr = (BYTE)(tag2 & 7);  // Y-value modifier
                cursorYRGB = (BYTE)(tag1 & 15);  // Cursor color
                okCursorType = ((tag1 & 16) != 0);  // TRUE - graphical cursor, FALSE - symbolic cursor
                ASSERT(okCursorType==0);  //DEBUG
                cursorPos = (BYTE)(((tag1 >> 8) >> scale) & 0x7f);  // Cursor position in the line
                cursorAddress = (BYTE)((tag1 >> 5) & 7);
                scale = 1 << scale;
            }
        }

        WORD addressBits = pBoard->GetRAMWord(0, address);  // The word before the last word - is address of bits from all three memory planes
        address += 2;

        // Calculate size, type and address of the next tag
        WORD tagB = pBoard->GetRAMWord(0, address);  // Last word of the tag - is address and type of the next tag
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
        if (yy < 19 /*|| yy > 306*/)
            continue;

        // Loop thru bits from addressBits, planes 0,1,2
        // For each pixel:
        //   Get bit from planes 0,1,2 and make value
        //   Map value to palette; result is 4-bit value YRGB
        //   Translate value to 24-bit RGB
        //   Put value to m_bits; repeat using scale value

        int xr = 640;
        int y = yy - 19;
        DWORD* pBits = (DWORD*) pScan;
        int pos = 0;
        for (;;)
        {
            // Get bit from planes 0,1,2
            BYTE src0 = pBoard->GetRAMByte(0, addressBits);
            BYTE src1 = pBoard->GetRAMByte(1, addressBits);
            BYTE src2 = pBoard->GetRAMByte(2, addressBits);
            // Loop through the bits of the byte
            int bit = 0;
            for (;;)
            {
                DWORD valueRGB;
                if (cursorOn && (pos == cursorPos) && (!okCursorType || (okCursorType && bit == cursorAddress)))
                    valueRGB = colors[cursorYRGB];  // 4-bit to 32-bit color
                else
				{
                    // Make 3-bit value from the bits
					BYTE value012 = (src0 & 1) | ((src1 & 1) << 1) | ((src2 & 1) << 2);
                    valueRGB = palettecurrent[value012];  // 3-bit to 32-bit color
				}

                // Put value to m_bits; repeat using scale value
                //WAS: for (int s = 0; s < scale; s++) *pBits++ = valueRGB;
                switch (scale)
                {
                case 8:
                    *pBits++ = valueRGB;
                    *pBits++ = valueRGB;
                    *pBits++ = valueRGB;
                    *pBits++ = valueRGB;
                case 4:
                    *pBits++ = valueRGB;
                    *pBits++ = valueRGB;
                case 2:
                    *pBits++ = valueRGB;
                case 1:
                    *pBits++ = valueRGB;
                default:
                    break;
                }

                xr -= scale;

                if (bit == 7)
                    break;
                bit++;

                // Shift to the next bit
                src0 >>= 1;
                src1 >>= 1;
                src2 >>= 1;
            }
            if (xr <= 0)
                break;  // End of line
            addressBits++;  // Go to the next byte
            pos++;
        }

        pScan += nStride;
    }
}

// Table for color conversion yrgb (4 bits) -> DWORD (32 bits)
const DWORD StandardRGBColors[16] = {
    0x000000, 0x000080, 0x008000, 0x008080, 0x800000, 0x800080, 0x808000, 0x808080,
    0x000000, 0x0000FF, 0x00FF00, 0x00FFFF, 0xFF0000, 0xFF00FF, 0xFFFF00, 0xFFFFFF,
};

void Emulator::RenderScreen(System::Drawing::Bitmap^ bitmap)
{
    using namespace System::Drawing;

    Rectangle rect = Rectangle(0, 0, bitmap->Width, bitmap->Height);

    Imaging::BitmapData^ bmpData = bitmap->LockBits(rect, Imaging::ImageLockMode::ReadWrite, Imaging::PixelFormat::Format32bppRgb);
    void* ptr = bmpData->Scan0.ToPointer();

    Emulator_PrepareScreenRGB32(m_pBoard, ptr, bmpData->Stride, StandardRGBColors);

    bitmap->UnlockBits(bmpData);
}
