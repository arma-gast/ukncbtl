// Common.cpp

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////


void AlertWarning(LPCTSTR sMessage)
{
#if defined(_WIN32)
    _tprintf(_T("! %s\n"), sMessage);
//TODO: Implement for other platforms
#endif
}

#if !defined(PRODUCT)

void DebugPrintFormat(LPCTSTR pszFormat, ...)
{
    //STUB
}

void DebugLog(LPCTSTR message)
{
    //STUB
}

void DebugLogFormat(LPCTSTR pszFormat, ...)
{
    //STUB
}

#endif // !defined(PRODUCT)


//////////////////////////////////////////////////////////////////////
