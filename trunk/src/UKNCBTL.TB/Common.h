/*  This file is part of UKNCBTL.
    UKNCBTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    UKNCBTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
UKNCBTL. If not, see <http://www.gnu.org/licenses/>. */

// Common.h

#pragma once


//////////////////////////////////////////////////////////////////////
// Assertions checking - MFC-like ASSERT macro

#ifdef _DEBUG

BOOL AssertFailedLine(LPCSTR lpszFileName, int nLine);
#define ASSERT(f)          (void) ((f) || !AssertFailedLine(__FILE__, __LINE__) || (DebugBreak(), 0))
#define VERIFY(f)          ASSERT(f)

#else   // _DEBUG

#define ASSERT(f)          ((void)0)
#define VERIFY(f)          ((void)f)

#endif // !_DEBUG


//////////////////////////////////////////////////////////////////////
// Alerts

void AlertWarning(LPCTSTR sMessage);
void AlertWarningFormat(LPCTSTR sFormat, ...);


//////////////////////////////////////////////////////////////////////
// DebugPrint

#if !defined(PRODUCT)

void DebugPrint(LPCTSTR message);
void DebugPrintFormat(LPCTSTR pszFormat, ...);
void DebugLogClear();
void DebugLog(LPCTSTR message);
void DebugLogFormat(LPCTSTR pszFormat, ...);

#endif // !defined(PRODUCT)


//////////////////////////////////////////////////////////////////////


// Processor register names
const TCHAR* REGISTER_NAME[];

void PrintOctalValue(TCHAR* buffer, WORD value);


//////////////////////////////////////////////////////////////////////


void Test_Log(char eventtype, LPCTSTR message);
void Test_LogFormat(char eventtype, LPCTSTR format, ...);
inline void Test_LogInfo(LPCTSTR message) { Test_Log('i', message); }
inline void Test_LogError(LPCTSTR message) { Test_Log('E', message); }

void Test_Init(LPCTSTR sTestTitle);
void Test_Done();

void Test_LoadROMCartridge(int slot, LPCTSTR sFilePath);
void Test_AttachFloppyImage(int slot, LPCTSTR sFilePath);
void Test_AttachHardImage(int slot, LPCTSTR sFilePath);
void Test_CreateHardImage(BYTE sectors, BYTE heads, int cylinders, LPCTSTR sFilePath);

void Test_OpenTape(LPCTSTR sFilePath);
void Test_CloseTape();

void Test_SaveScreenshot(LPCTSTR sFileName);
void Test_CheckScreenshot(LPCTSTR sFileName);

void Test_CopyFile(LPCTSTR sFileNameFrom, LPCTSTR sFileNameTo);
void Test_CreateDiskImage(LPCTSTR sFileName, int tracks);


//////////////////////////////////////////////////////////////////////
