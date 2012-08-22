// UKNCBTL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include "UKNCBTL.h"
#include "Emulator.h"


/////////////////////////////////////////////////////////////////////////////
// Globals

SDL_Surface*    g_Screen = NULL;
SDL_Surface*    g_UkncScreen = NULL;
int             g_UkncScreenWid = UKNC_SCREEN_WIDTH, g_UkncScreenHei = UKNC_SCREEN_HEIGHT;
int             g_okQuit = FALSE;
int             g_LastDelay = 0;        //DEBUG: Last delay value, milliseconds
int             g_LastFps = 0;          //DEBUG: Last Frames-per-Second value

#define FRAME_TICKS             40      // 1000 us / 40 = 25 frames per second


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
    SDL_UnlockSurface(g_UkncScreen);
    
    // Draw UKNC screen
    SDL_Rect src, dst, rc;
    src.x = src.y = dst.x = dst.y = 0;
    src.w = dst.w = SCREEN_WIDTH;
    src.h = dst.w = SCREEN_HEIGHT;
    if (g_UkncScreenWid < SCREEN_WIDTH)
    {
        int scrleft = (SCREEN_WIDTH - g_UkncScreenWid) / 2;
        src.w = dst.w = g_UkncScreenWid;  dst.x = scrleft;
        rc.x = rc.y = 0;  rc.w = scrleft;  rc.h = SCREEN_HEIGHT;
        SDL_FillRect(g_Screen, &rc, 0);
        rc.x = scrleft + g_UkncScreenWid;  rc.y = 0;  rc.w = SCREEN_WIDTH - scrleft - g_UkncScreenWid;  rc.h = SCREEN_HEIGHT;
        SDL_FillRect(g_Screen, &rc, 0);
    }
    if (g_UkncScreenHei < SCREEN_HEIGHT)
    {
        src.h = dst.h = g_UkncScreenHei;
        dst.y = (SCREEN_HEIGHT - g_UkncScreenHei) / 2;
    }
    SDL_BlitSurface(g_UkncScreen, &src, g_Screen, &dst);
    
    //if (g_okKeyboard)
    //    Main_DrawKeyboard();

    SDL_Flip(g_Screen);
}


/////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
#  undef main  //HACK for VC error LNK1561: entry point must be defined
#endif
int main(int argc, char* argv[])
{
    // Init SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 255;  // Unable to initialize SDL

#ifdef _WIN32
    SDL_putenv("SDL_VIDEODRIVER=windib");
    SDL_putenv("SDL_DEBUG=1");
    SDL_putenv("SDL_VIDEO_WINDOW_POS=300,200");
    SDL_WM_SetCaption("UKNCBTL SDL", "UKNCBTL SDL");
#else
    SDL_ShowCursor(SDL_DISABLE);
#endif

    // Prepare screen surface
    g_Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITPERPIXEL, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (g_Screen == NULL)
        return 254;  // Unable to set video mode
    g_UkncScreen = SDL_CreateRGBSurface(0, g_UkncScreenWid, g_UkncScreenHei, 32, 0,0,0,0);
    if (g_Screen == NULL)
        return 253;

    if (!Emulator_Init())
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
                    //TODO: Main_OnKeyJoyEvent(evt);
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
    Emulator_Done();

    SDL_FreeSurface(g_Screen);
    SDL_FreeSurface(g_UkncScreen);

    SDL_Quit();

    return 0;
}

