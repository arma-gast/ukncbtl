// UKNCBTL.h

#pragma once

#include <map>


/////////////////////////////////////////////////////////////////////////////


#define VERSION_MAJOR           0
#define VERSION_MINOR           35

#define SCREEN_WIDTH            800
#define SCREEN_HEIGHT           600
#define SCREEN_BITPERPIXEL      32


/////////////////////////////////////////////////////////////////////////////


extern const TCHAR* SETTINGS_SECTION_SDL;
extern const TCHAR* SETTINGS_SECTION_UKNCBTL;
extern const TCHAR* SETTINGS_KEY_CARTRIDGE1;
extern const TCHAR* SETTINGS_KEY_FLOPPY0;
extern const TCHAR* SETTINGS_KEY_HARD1;

typedef std::map<TSTRING, TSTRING> TSETTINGSKEYVALUEMAP;

// Parse the INI file
void Settings_ParseIniFile(const TSTRING& sFileName);
// Get value for the given section and key
TSTRING Settings_GetValue(const TCHAR* section, const TCHAR* key);
// Get all key-value pairs for the given section
const TSETTINGSKEYVALUEMAP* Settings_GetKeyValues(const TCHAR* section);


/////////////////////////////////////////////////////////////////////////////
