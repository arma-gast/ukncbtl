#ifndef COMMON_H
#define COMMON_H
// Common.h

#pragma once

#include <string>


//////////////////////////////////////////////////////////////////////
// Defines for compilation under MinGW and GCC

#ifndef _TCHAR_DEFINED
#ifdef	_UNICODE
typedef wchar_t TCHAR;
#define _tfopen     _wfopen
#define _tfsopen    _wfsopen
#define _tcscpy     wcscpy
#define _tstat      _wstat
#define _tcsrchr    wcsrchr
#define _tcsicmp    _wcsicmp
#define _sntprintf  _snwprintf
#define _tprintf    wprintf
#define _tcslen     wcslen
#define _stscanf    swscanf
#else
typedef char TCHAR;
#define _tfopen     fopen
#define _tfsopen    _fsopen
#define _tcscpy     strcpy
#define _tstat      _stat
#define _tcsrchr    strrchr
#define _tcsicmp    _stricmp
#define _sntprintf  _snprintf
#define _tprintf    printf
#define _tcslen     strlen
#define _stscanf    sscanf
#endif
#define _T(x)       x
#endif

#ifdef __GNUG__
#define _stat       stat
//#define _stricmp    strcasecmp
#define _snprintf   snprintf
#endif

#ifdef	_UNICODE
typedef const wchar_t * LPCTSTR;
#else
typedef const char * LPCTSTR;
#endif

typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef int BOOL;
#ifndef FALSE
#define FALSE               0
#endif
#ifndef TRUE
#define TRUE                1
#endif

#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))

#define CALLBACK

typedef void *HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG)-1)
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

typedef std::basic_string<TCHAR> TSTRING;


//////////////////////////////////////////////////////////////////////
// Assertions checking - MFC-like ASSERT macro

#define ASSERT(f)          ((void)0)
#define VERIFY(f)          ((void)f)


//////////////////////////////////////////////////////////////////////


void AlertWarning(LPCTSTR sMessage);
BOOL AlertOkCancel(LPCTSTR sMessage);


//////////////////////////////////////////////////////////////////////
// DebugPrint

#if !defined(PRODUCT)

void DebugPrint(LPCTSTR message);
void DebugPrintFormat(LPCTSTR pszFormat, ...);
void DebugLog(LPCTSTR message);
void DebugLogFormat(LPCTSTR pszFormat, ...);

#endif // !defined(PRODUCT)


//////////////////////////////////////////////////////////////////////


// Processor register names
const LPCTSTR REGISTER_NAME[] = { _T("R0"), _T("R1"), _T("R2"), _T("R3"), _T("R4"), _T("R5"), _T("SP"), _T("PC") };

const int UKNC_SCREEN_WIDTH = 640;
const int UKNC_SCREEN_HEIGHT = 288;

//void Common_Cleanup();
void PrintOctalValue(TCHAR* buffer, WORD value);
void PrintBinaryValue(TCHAR* buffer, WORD value);
//void DrawOctalValue(QPainter &painter, int x, int y, WORD value);
//void DrawBinaryValue(QPainter &painter, int x, int y, WORD value);
BOOL ParseOctalValue(LPCTSTR text, WORD* pValue);


//////////////////////////////////////////////////////////////////////
#endif // COMMON_H
