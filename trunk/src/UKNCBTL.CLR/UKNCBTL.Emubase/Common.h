// Common.h

#pragma once


//////////////////////////////////////////////////////////////////////
// Data types

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

typedef const TCHAR *LPCTSTR;


//////////////////////////////////////////////////////////////////////
// Assertions checking - MFC-like ASSERT macro

#ifdef _DEBUG

BOOL AssertFailedLine(const char* lpszFileName, int nLine);
#define ASSERT(f)          (void) ((f))  //TODO: Implement it
#define VERIFY(f)          ASSERT(f)

#else   // _DEBUG

#define ASSERT(f)          ((void)0)
#define VERIFY(f)          ((void)f)

#endif // !_DEBUG



//////////////////////////////////////////////////////////////////////


// Processor register names
const TCHAR* REGISTER_NAME[];
