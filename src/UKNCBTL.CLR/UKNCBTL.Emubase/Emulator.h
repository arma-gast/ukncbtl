#pragma once

#include "emubase\Board.h"

namespace UKNCBTL
{

public ref class Emulator
{
private:
    CMotherboard* m_pBoard;
    bool m_okRunning;
    long m_nUptimeFrameCount;
    unsigned long m_dwEmulatorUptime;  // UKNC uptime, seconds, from turn on or reset, increments every 25 frames
    System::UInt16 m_wCPUBreakpoint;
    System::UInt16 m_wPPUBreakpoint;

public:
    Emulator();
    ~Emulator();

public:
    void SetCPUBreakpoint(System::UInt16 address);
    void SetPPUBreakpoint(System::UInt16 address);
    void Start();
    void Stop();
    void Reset();
    bool IsRunning() { return m_okRunning; }
    float GetUptime();
    bool SystemFrame();

    bool LoadROM(System::String ^sFilePath);
    //bool LoadROMCartridge(int slot, System::String ^sFilePath);

    void RenderScreen(System::Drawing::Bitmap ^bitmap);
};

}  // namespace
