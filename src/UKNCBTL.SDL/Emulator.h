// Emulator.h

#pragma once

#include "emubase/Board.h"

//////////////////////////////////////////////////////////////////////

extern CMotherboard* g_pBoard;
extern BOOL g_okEmulatorRunning;


BOOL Emulator_Init();
void Emulator_Done();
void Emulator_Start();
void Emulator_Stop();
void Emulator_Reset();

int  Emulator_SystemFrame();
void Emulator_PrepareScreenRGB32(void* pImageBits, const DWORD* colors);

void Emulator_KeyboardEvent(BYTE key, int okPressed);

BOOL Emulator_LoadROMCartridge(int slot, LPCTSTR sFilePath);
BOOL Emulator_AttachFloppy(int slot, LPCTSTR sFilePath);
BOOL Emulator_AttachHardDrive(int slot, LPCTSTR sFilePath);


//////////////////////////////////////////////////////////////////////
