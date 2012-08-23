// UKNCBTL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include "UKNCBTL.h"
#include "Emulator.h"


/////////////////////////////////////////////////////////////////////////////
// Globals

TSTRING         g_ApplicationExePath;
TSTRING         g_ApplicationIniPath;
int             g_ScreenWidth = 800;
int             g_ScreenHeight = 600;
int             g_ScreenBitsPerPixel = 0;
bool            g_FullScreen = false;
SDL_Surface*    g_Screen = NULL;
SDL_Surface*    g_UkncScreen = NULL;
int             g_UkncScreenWid = UKNC_SCREEN_WIDTH, g_UkncScreenHei = UKNC_SCREEN_HEIGHT;
int             g_ScreenMode = 0;
int             g_okQuit = FALSE;
int             g_LastDelay = 0;        //DEBUG: Last delay value, milliseconds
int             g_LastFps = 0;          //DEBUG: Last Frames-per-Second value

#define FRAME_TICKS             40      // 1000 us / 40 = 25 frames per second


void Main_ExecuteCommand(int command);


/////////////////////////////////////////////////////////////////////////////

enum EventType
{
    EVKEY = 1,
    EVJOY = 2,
    EVCMD = 3,
};

struct KeyMappingStruct
{
    int             sourcetype;         // Source: 0 - none, 1 - keyboard
    int             sourcecd;           // Source code: keyboard - SDLK_Xxx
    int             resulttype;         // Result: 0 - none, 1 - UKNC keyboard, 3 - command
    unsigned int    resultcd;           // Result code: keyboard - key scan, command - ID
};

static KeyMappingStruct g_KeyMapping[] = {
// Top line
    { EVKEY, SDLK_F1,       EVKEY,  0010 },
    { EVKEY, SDLK_F2,       EVKEY,  0011 },
    { EVKEY, SDLK_F3,       EVKEY,  0012 },
    { EVKEY, SDLK_F4,       EVKEY,  0014 },
    { EVKEY, SDLK_F5,       EVKEY,  0015 },
    { EVKEY, SDLK_F6,       EVKEY,  0152 },  // œŒÃ
    { EVKEY, SDLK_F7,       EVKEY,  0152 },  // ”—“
    { EVKEY, SDLK_F8,       EVKEY,  0151 },  // »—œ
    { EVKEY, SDLK_F9,       EVKEY,  0171 },  // —¡–Œ—
    { EVKEY, SDLK_F10,      EVKEY,  0004 },  // —“Œœ
// 1st line -- digits
    { EVKEY, SDLK_BACKQUOTE,EVKEY,  0006 },  // AP2
    { EVKEY, SDLK_SEMICOLON,EVKEY,  0007 },
    { EVKEY, SDLK_1,        EVKEY,  0030 },
    { EVKEY, SDLK_2,        EVKEY,  0031 },
    { EVKEY, SDLK_3,        EVKEY,  0032 },
    { EVKEY, SDLK_4,        EVKEY,  0013 },
    { EVKEY, SDLK_5,        EVKEY,  0034 },
    { EVKEY, SDLK_6,        EVKEY,  0035 },
    { EVKEY, SDLK_7,        EVKEY,  0016 },
    { EVKEY, SDLK_8,        EVKEY,  0017 },
    { EVKEY, SDLK_9,        EVKEY,  0177 },
    { EVKEY, SDLK_0,        EVKEY,  0176 },
    { EVKEY, SDLK_MINUS,    EVKEY,  0175 },
    { EVKEY, SDLK_SLASH,    EVKEY,  0173 },  // Slash / ?
    { EVKEY, SDLK_BACKSPACE,EVKEY,  0132 },  // «¡
// 2nd line
    { EVKEY, SDLK_TAB,      EVKEY,  0026 },
    { EVKEY, SDLK_j,        EVKEY,  0027 },
    { EVKEY, SDLK_c,        EVKEY,  0050 },
    { EVKEY, SDLK_u,        EVKEY,  0051 },
    { EVKEY, SDLK_k,        EVKEY,  0052 },
    { EVKEY, SDLK_e,        EVKEY,  0033 },
    { EVKEY, SDLK_n,        EVKEY,  0054 },
    { EVKEY, SDLK_g,        EVKEY,  0055 },
    { EVKEY, SDLK_LEFTBRACKET,EVKEY,0036 },
    { EVKEY, SDLK_RIGHTBRACKET,EVKEY,0037 },
    { EVKEY, SDLK_z,        EVKEY,  0157 },
    { EVKEY, SDLK_h,        EVKEY,  0156 },
    { EVKEY, SDLK_COLON,    EVKEY,  0155 },
// 3rd line
    //TODO: ”œ–
    { EVKEY, SDLK_f,        EVKEY,  0047 },
    { EVKEY, SDLK_y,        EVKEY,  0070 },
    { EVKEY, SDLK_w,        EVKEY,  0071 },
    { EVKEY, SDLK_a,        EVKEY,  0072 },
    { EVKEY, SDLK_p,        EVKEY,  0053 },
    { EVKEY, SDLK_r,        EVKEY,  0074 },
    { EVKEY, SDLK_o,        EVKEY,  0075 },
    { EVKEY, SDLK_l,        EVKEY,  0056 },
    { EVKEY, SDLK_d,        EVKEY,  0057 },
    { EVKEY, SDLK_v,        EVKEY,  0137 },
    { EVKEY, SDLK_BACKSLASH,EVKEY,  0136 },  // › / Backslash
    { EVKEY, SDLK_PERIOD,   EVKEY,  0135 },
    { EVKEY, SDLK_RETURN,   EVKEY,  0153 },
// 4th line
    //TODO: ¿À‘
    //TODO: √–¿‘
    { EVKEY, SDLK_q,        EVKEY,  0067 },
    //TODO: ◊ / ^
    { EVKEY, SDLK_s,        EVKEY,  0111 },
    { EVKEY, SDLK_m,        EVKEY,  0112 },
    { EVKEY, SDLK_i,        EVKEY,  0073 },
    { EVKEY, SDLK_t,        EVKEY,  0114 },
    { EVKEY, SDLK_x,        EVKEY,  0115 },
    { EVKEY, SDLK_b,        EVKEY,  0076 },
    //TODO: ﬁ / @
    { EVKEY, SDLK_COMMA,    EVKEY,  0117 },
// Last line
    { EVKEY, SDLK_LSHIFT,   EVKEY,  0105 },
    //TODO: ‘» —
    { EVKEY, SDLK_SPACE,    EVKEY,  0113 },
    { EVKEY, SDLK_RSHIFT,   EVKEY,  0105 },
    { EVKEY, SDLK_DOWN,     EVKEY,  0134 },
    { EVKEY, SDLK_UP,       EVKEY,  0154 },
    { EVKEY, SDLK_LEFT,     EVKEY,  0116 },
    { EVKEY, SDLK_RIGHT,    EVKEY,  0133 },
// Commands
    { EVKEY, SDLK_ESCAPE,   EVCMD,  ID_EXIT },
};

// Search for suitable mapping.
//  sourcetype: 1 - keyboard, 2 - joystick
//  ismenu: 0 - search for keyboard/joystick result; 1 - search for menu result
KeyMappingStruct* FindKeyMapping(int sourcetype, int sourcecd, int ismenu)
{
    for (int i = 0; i < sizeof(g_KeyMapping) / sizeof(KeyMappingStruct); i++)
    {
        if (g_KeyMapping[i].sourcetype == sourcetype &&
            g_KeyMapping[i].sourcecd == sourcecd)
        {
            if (!ismenu &&
                (g_KeyMapping[i].resulttype == EVKEY || g_KeyMapping[i].resulttype == EVJOY))
                return g_KeyMapping + i;
            if (ismenu && g_KeyMapping[i].resulttype == EVCMD)
                return g_KeyMapping + i;
        }
    }

    return NULL;  // Mapping not found
}

void Main_OnKeyJoyEvent(SDL_Event evt)
{
    KeyMappingStruct* mapping;
    int pressed = (evt.type == SDL_KEYDOWN || evt.type == SDL_JOYBUTTONDOWN);
    int sourcetype = (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP) ? EVKEY : EVJOY;

    mapping = FindKeyMapping(sourcetype, evt.key.keysym.sym, FALSE);
    if (mapping != NULL)  // UKNC event mapping found
    {
        BYTE result = mapping->resultcd;
        Emulator_KeyboardEvent(result, pressed);
    }
    else if (pressed)  // Commands works only on key/button press, not release
    {
        mapping = FindKeyMapping(sourcetype, evt.key.keysym.sym, TRUE);
        if (mapping != NULL)  // Command mapping found
        {
            Main_ExecuteCommand(mapping->resultcd);
        }
    }
}

void Main_ExecuteCommand(int command)
{
    switch (command)
    {
    case ID_EXIT:
        g_okQuit = TRUE;
        break;
    default:
        break;
    }
}


/////////////////////////////////////////////////////////////////////////////

// Table for color conversion yrgb (4 bits) -> DWORD (32 bits)
const DWORD ScreenView_StandardRGBColors[16] = {
    0x000000, 0x000080, 0x008000, 0x008080, 0x800000, 0x800080, 0x808000, 0x808080,
    0x000000, 0x0000FF, 0x00FF00, 0x00FFFF, 0xFF0000, 0xFF00FF, 0xFFFF00, 0xFFFFFF,
};

void Main_DrawScreen()
{
    SDL_LockSurface(g_UkncScreen);
    void* pPixels = g_UkncScreen->pixels;
    Emulator_PrepareScreenRGB32(pPixels, ScreenView_StandardRGBColors);
    Screen_UpscaleScreen(pPixels, g_ScreenMode);
    SDL_UnlockSurface(g_UkncScreen);
    
    // Draw UKNC screen
    SDL_Rect src, dst, rc;
    src.x = src.y = dst.x = dst.y = 0;
    src.w = dst.w = g_ScreenWidth;
    src.h = dst.w = g_ScreenHeight;
    if (g_UkncScreenWid < g_ScreenWidth)
    {
        int scrleft = (g_ScreenWidth - g_UkncScreenWid) / 2;
        src.w = dst.w = g_UkncScreenWid;  dst.x = scrleft;
        rc.x = rc.y = 0;  rc.w = scrleft;  rc.h = g_ScreenHeight;
        SDL_FillRect(g_Screen, &rc, 0);
        rc.x = scrleft + g_UkncScreenWid;  rc.y = 0;  rc.w = g_ScreenWidth - scrleft - g_UkncScreenWid;  rc.h = g_ScreenHeight;
        SDL_FillRect(g_Screen, &rc, 0);
    }
    if (g_UkncScreenHei < g_ScreenHeight)
    {
        src.h = dst.h = g_UkncScreenHei;
        dst.y = (g_ScreenHeight - g_UkncScreenHei) / 2;
    }
    SDL_BlitSurface(g_UkncScreen, &src, g_Screen, &dst);
    
    //if (g_okKeyboard)
    //    Main_DrawKeyboard();

    SDL_Flip(g_Screen);
}


/////////////////////////////////////////////////////////////////////////////

// Initialize the application
bool InitInstance()
{
    // Parse INI file
    Settings_ParseIniFile(g_ApplicationIniPath);

    // Retrieve general settings
    g_ScreenWidth = Settings_GetValueInt(SETTINGS_SECTION_VIDEO, SETTINGS_KEY_WIDTH, 800);
    if (g_ScreenWidth < 320) g_ScreenWidth = 320;
    g_ScreenHeight = Settings_GetValueInt(SETTINGS_SECTION_VIDEO, SETTINGS_KEY_HEIGHT, 600);
    if (g_ScreenHeight < 240) g_ScreenHeight = 240;
    g_ScreenBitsPerPixel = Settings_GetValueInt(SETTINGS_SECTION_VIDEO, SETTINGS_KEY_BITSPERPIXEL, 0);
    if (g_ScreenBitsPerPixel < 0) g_ScreenBitsPerPixel = 0;
    g_FullScreen = Settings_GetValueBool(SETTINGS_SECTION_VIDEO, SETTINGS_KEY_FULLSCREEN, false);

    g_ScreenMode = Settings_GetValueInt(SETTINGS_SECTION_VIDEO, SETTINGS_KEY_SCREENMODE, 0);
    Screen_GetScreenSize(g_ScreenMode, &g_UkncScreenWid, &g_UkncScreenHei);
    if (g_UkncScreenWid == 0 || g_UkncScreenHei == 0)  // Unallowed mode, reset to default
    {
        g_ScreenMode = 0;
        Screen_GetScreenSize(g_ScreenMode, &g_UkncScreenWid, &g_UkncScreenHei);
    }

    // Init SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;  // Unable to initialize SDL
    
#ifdef _WIN32
    SDL_WM_SetCaption("UKNCBTL SDL", "UKNCBTL SDL");
#else
    SDL_ShowCursor(SDL_DISABLE);
#endif

    // Apply SDL settings from the "SDL" section of the INI file
    // See http://www.libsdl.org/docs/html/sdlenvvars.html for the list of options
    const TSETTINGSKEYVALUEMAP* pMapSdl = Settings_GetKeyValues(SETTINGS_SECTION_SDL);
    if (pMapSdl != 0)
    {
        for (TSETTINGSKEYVALUEMAP::const_iterator itsdl = pMapSdl->begin(); itsdl != pMapSdl->end(); ++itsdl)
        {
            TCHAR buffer[256];
            _sntprintf(buffer, 256, _T("%s=%s"), itsdl->first.c_str(), itsdl->second.c_str());
            SDL_putenv(buffer);
        }
    }

    // Prepare screen surface
    int flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    if (g_FullScreen) flags |= SDL_FULLSCREEN;
    g_Screen = SDL_SetVideoMode(g_ScreenWidth, g_ScreenHeight, g_ScreenBitsPerPixel, flags);
    if (g_Screen == NULL)
        return false;  // Unable to set video mode
    g_UkncScreen = SDL_CreateRGBSurface(0, g_UkncScreenWid, g_UkncScreenHei, 32, 0,0,0,0);
    if (g_Screen == NULL)
        return false;

    if (!Emulator_Init())
        return false;

    // Restore settings: cartridges
    for (int slot = 1; slot < 2; slot++)
    {
        TCHAR buffer[11];
        _tcscpy(buffer, SETTINGS_KEY_CARTRIDGE1);
        buffer[_tcslen(buffer) - 1] = _T('0') + slot;
        TSTRING sFileName = Settings_GetValue(SETTINGS_SECTION_UKNCBTL, buffer);
        if (! sFileName.empty())
            Emulator_LoadROMCartridge(slot, sFileName.c_str());
    }
    // Restore settings: floppies
    for (int slot = 0; slot < 4; slot++)
    {
        TCHAR buffer[11];
        _tcscpy(buffer, SETTINGS_KEY_FLOPPY0);
        buffer[_tcslen(buffer) - 1] = _T('0') + slot;
        TSTRING sFileName = Settings_GetValue(SETTINGS_SECTION_UKNCBTL, buffer);
        if (! sFileName.empty())
            Emulator_AttachFloppy(slot, sFileName.c_str());
    }
    // Restore settings: hard drives
    for (int slot = 1; slot < 2; slot++)
    {
        TCHAR buffer[11];
        _tcscpy(buffer, SETTINGS_KEY_HARD1);
        buffer[_tcslen(buffer) - 1] = _T('0') + slot;
        TSTRING sFileName = Settings_GetValue(SETTINGS_SECTION_UKNCBTL, buffer);
        if (! sFileName.empty())
            Emulator_AttachHardDrive(slot, sFileName.c_str());
    }

    ////DEBUG: SDL key names
    //std::map<const char*, int> keys;
    //for (int code = 0; code < 512; code++)
    //{
    //    const char * keyname = SDL_GetKeyName((SDLKey)code);
    //    if (_stricmp(keyname, "unknown key") != 0)
    //        keys[keyname] = code;
    //}
    //for (std::map<const char*, int>::iterator it = keys.begin(); it != keys.end(); ++it)
    //{
    //    printf("%04x\t%s\n", it->second, it->first);
    //}
    //printf("TOTAL: %d\n", keys.size());

    return true;
}

void DoneInstance()
{
    Emulator_Done();

    SDL_FreeSurface(g_Screen);
    SDL_FreeSurface(g_UkncScreen);

    SDL_Quit();
}

#if defined(_WIN32)
#  undef main  //HACK for VC error LNK1561: entry point must be defined
#endif
int main(int argc, char* argv[])
{
    // Get application path
    g_ApplicationExePath.append(argv[0]);

    // Calculate INI file path
    g_ApplicationIniPath = g_ApplicationExePath;
    g_ApplicationIniPath.resize(g_ApplicationIniPath.find_last_of(_T('.')));
    g_ApplicationIniPath.append(_T(".ini"));

    if (! InitInstance())
        return 255;

    Emulator_Start();

    // The main loop
    Uint32 ticksLast;
    Uint32 ticksLastFps = SDL_GetTicks();
    int frames = 0;
    while (!g_okQuit)
    {
        ticksLast = SDL_GetTicks();  // Time at frame start
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                g_okQuit = TRUE;
                break;
            }
            else
            {
                if (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP ||
                    evt.type == SDL_JOYBUTTONDOWN || evt.type == SDL_JOYBUTTONUP)
                {
                    Main_OnKeyJoyEvent(evt);
                }
            }
        }

        if (g_okEmulatorRunning)
        {
            Emulator_SystemFrame();

            Main_DrawScreen();
            
            frames++;
        }

        // Delay
        Uint32 ticksNew = SDL_GetTicks();
        Uint32 ticksElapsed = ticksNew - ticksLast;
        g_LastDelay = 0;
        if (ticksElapsed < FRAME_TICKS)
        {
            g_LastDelay = FRAME_TICKS - ticksElapsed;
            SDL_Delay(FRAME_TICKS - ticksElapsed);
        }
        ticksLast = ticksNew;
        
        if (ticksLast - ticksLastFps > 1000)  //DEBUG: FPS calculation
        {
            g_LastFps = frames;
            frames = 0;
            ticksLastFps += 1000;
        }
    }

    Emulator_Stop();

    DoneInstance();

    return 0;
}

