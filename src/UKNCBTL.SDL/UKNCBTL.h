// UKNCBTL.h

#pragma once

#include <map>


/////////////////////////////////////////////////////////////////////////////


#define VERSION_MAJOR           0
#define VERSION_MINOR           35


//////////////////////////////////////////////////////////////////////
// Commands

#define ID_EXIT                 1001


/////////////////////////////////////////////////////////////////////////////
// Settings

extern const TCHAR* SETTINGS_SECTION_SDL;
extern const TCHAR* SETTINGS_SECTION_VIDEO;
extern const TCHAR* SETTINGS_SECTION_UKNCBTL;
extern const TCHAR* SETTINGS_KEY_CARTRIDGE1;
extern const TCHAR* SETTINGS_KEY_FLOPPY0;
extern const TCHAR* SETTINGS_KEY_HARD1;
extern const TCHAR* SETTINGS_KEY_WIDTH;
extern const TCHAR* SETTINGS_KEY_HEIGHT;
extern const TCHAR* SETTINGS_KEY_BITSPERPIXEL;
extern const TCHAR* SETTINGS_KEY_FULLSCREEN;
extern const TCHAR* SETTINGS_KEY_SCREENMODE;

typedef std::map<TSTRING, TSTRING> TSETTINGSKEYVALUEMAP;

// Parse the INI file
void Settings_ParseIniFile(const TSTRING& sFileName);
// Get value for the given section and key
TSTRING Settings_GetValue(const TCHAR* section, const TCHAR* key);
// Get all key-value pairs for the given section
const TSETTINGSKEYVALUEMAP* Settings_GetKeyValues(const TCHAR* section);
// Get boolean value for the given section and key
bool Settings_GetValueBool(const TCHAR* section, const TCHAR* key, bool defaultValue = false);
// Get integer value for the given section and key
int Settings_GetValueInt(const TCHAR* section, const TCHAR* key, int defaultValue = 0);


/////////////////////////////////////////////////////////////////////////////
// Screen

void Screen_GetScreenSize(int scrmode, int* pwid, int* phei);
void Screen_UpscaleScreen(void* pBits, int screenMode);


/////////////////////////////////////////////////////////////////////////////
