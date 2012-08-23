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

struct
{
    int  source;           // Source code: SDLK_Xxx
    BYTE resultlat;        // Result code: key scan
    BYTE resultrus;        // Result code: key scan
}
static g_KeyMapping[] = {
    { SDLK_1,           0030, 0030 },
    { SDLK_2,           0031, 0031 },
    { SDLK_3,           0032, 0032 },
    { SDLK_4,           0013, 0013 },
    { SDLK_5,           0034, 0034 },
    { SDLK_6,           0035, 0035 },
    { SDLK_7,           0016, 0016 },
    { SDLK_8,           0017, 0017 },
    { SDLK_9,           0177, 0177 },
    { SDLK_0,           0176, 0176 },
    { SDLK_BACKSPACE,   0132, 0132 },  // ÇÁ
    { SDLK_TAB,         0026, 0026 },
    { SDLK_RETURN,      0153, 0153 },
    { SDLK_SPACE,       0113, 0113 },
    { SDLK_DOWN,        0134, 0134 },
    { SDLK_UP,          0154, 0154 },
    { SDLK_LEFT,        0116, 0116 },
    { SDLK_RIGHT,       0133, 0133 },
    { SDLK_F1,          0010, 0010 },
    { SDLK_F2,          0011, 0011 },
    { SDLK_F3,          0012, 0012 },
    { SDLK_F4,          0014, 0014 },
    { SDLK_F5,          0015, 0015 },
    { SDLK_F6,          0172, 0172 },  // ÏÎÌ
    { SDLK_F7,          0152, 0152 },  // ÓÑÒ
    { SDLK_F8,          0151, 0151 },  // ÈÑÏ
    { SDLK_F9,          0171, 0171 },  // ÑÁÐÎÑ
    { SDLK_F11,         0004, 0004 },  // ÑÒÎÏ
    { SDLK_INSERT,      0171, 0171 },  // ÑÁÐÎÑ
    { SDLK_PAGEUP,      0004, 0004 },  // ÑÒÎÏ
    { SDLK_DELETE,      0172, 0172 },  // ÏÎÌ
    { SDLK_END,         0152, 0152 },  // ÓÑÒ
    { SDLK_PAGEDOWN,    0151, 0151 },  // ÈÑÏ
// Others
    //TODO: AP2
    { SDLK_LCTRL,       0046, 0046 },  // LCtrl  =>    ÓÏÐ
    { SDLK_RCTRL,       0066, 0066 },  // RCtrl  =>    ÃÐÀÔ
    { SDLK_MENU,        0106, 0106 },  // Menu   =>    ÀËÔ
    { SDLK_CAPSLOCK,    0107, 0107 },  // ÔÈÊÑ
    { SDLK_LSHIFT,      0105, 0105 },
    { SDLK_RSHIFT,      0105, 0105 },
    { SDLK_MINUS,       0175, 0175 },
    { SDLK_SLASH,       0173, 0173 },  // Slash / ?
    { SDLK_BACKQUOTE,   0007, 0007 },
// Changing on RUS/LAT switch
    { SDLK_a,           0072, 0047 },
    { SDLK_b,           0076, 0073 },
    { SDLK_c,           0050, 0111 },
    { SDLK_d,           0057, 0071 },
    { SDLK_e,           0033, 0051 },
    { SDLK_f,           0047, 0072 },
    { SDLK_g,           0055, 0053 },
    { SDLK_h,           0156, 0074 },
    { SDLK_i,           0073, 0036 },
    { SDLK_j,           0027, 0075 },
    { SDLK_k,           0052, 0056 },
    { SDLK_l,           0056, 0057 },
    { SDLK_m,           0112, 0115 },
    { SDLK_n,           0054, 0114 },
    { SDLK_o,           0075, 0037 },
    { SDLK_p,           0053, 0157 },
    { SDLK_q,           0067, 0027 },
    { SDLK_r,           0074, 0052 },
    { SDLK_s,           0111, 0070 },
    { SDLK_t,           0114, 0033 },
    { SDLK_u,           0051, 0055 },
    { SDLK_v,           0137, 0112 },
    { SDLK_w,           0071, 0050 },
    { SDLK_x,           0115, 0110 },
    { SDLK_y,           0070, 0054 },
    { SDLK_z,           0157, 0067 },
    { SDLK_LEFTBRACKET, 0036, 0156 },  // [      =>   [   Õ
    { SDLK_RIGHTBRACKET,0037, 0155 },  // ]      =>   ]   Ú
    { SDLK_HOME,        0155, 0174 },  // Home
    { SDLK_SEMICOLON,   0155, 0174 },
    { SDLK_COMMA,       0117, 0076 },  // ,<     =>   ,<  Áá
    { SDLK_PERIOD,      0135, 0077 },  // .>     =>   .>  Þþ
    { SDLK_BACKSLASH,   0136, 0135 },  // \|     =>   \|  .>
    { SDLK_QUOTE,       0077, 0136 },  // '"     =>   @`  Ýý
    { 0,                0,    0    }
};

struct
{
    int source;           // Source code: SDLK_Xxx
    int result;           // Result code: command ID
}
static g_CommandMapping[] = {
    { SDLK_ESCAPE,      ID_EXIT },
    { 0,                0 }
};

BYTE FindKeyMapping(int sourcecd)
{
    for (int i = 0; ; i++)
    {
        if (g_KeyMapping[i].source == 0)
            break;  // End of the table
        if (g_KeyMapping[i].source == sourcecd)
        {
            bool latrus = ((g_pBoard->GetKeyboardRegister() & KEYB_LAT) != 0);
            return latrus ? g_KeyMapping[i].resultlat : g_KeyMapping[i].resultrus;
        }
    }
    return 0;  // Mapping not found
}

int FindCommandMapping(int sourcecd)
{
    for (int i = 0; ; i++)
    {
        if (g_CommandMapping[i].source == 0)
            break;  // End of the table
        if (g_CommandMapping[i].source == sourcecd)
            return g_CommandMapping[i].result;
    }
    return 0;  // Mapping not found
}

void Main_OnKeyJoyEvent(SDL_Event evt)
{
    int pressed = (evt.type == SDL_KEYDOWN || evt.type == SDL_JOYBUTTONDOWN);
    //int sourcetype = (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP) ? EVKEY : EVJOY;

    //DEBUG
    if (pressed) printf("KeyDown %d\n", evt.key.keysym.sym);

    BYTE ukncscan = FindKeyMapping(evt.key.keysym.sym);
    if (ukncscan != 0)  // UKNC event mapping found
    {
        Emulator_KeyboardEvent(ukncscan, pressed);
    }
    else if (pressed)  // Commands works only on key/button press, not release
    {
        int command = FindCommandMapping(evt.key.keysym.sym);
        if (command != 0)  // Command mapping found
        {
            Main_ExecuteCommand(command);
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

