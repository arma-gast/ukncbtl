// Screen.cpp

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

//Прототип функции преобразования экрана
typedef void (CALLBACK* PREPARE_SCREEN_CALLBACK)(void* pImageBits);

void Screen_UpscaleScreen1(void* pImageBits);
void Screen_UpscaleScreen2(void* pImageBits);
void Screen_UpscaleScreen3(void* pImageBits);
void Screen_UpscaleScreen4(void* pImageBits);

struct ScreenModeStruct
{
    int width;
    int height;
    PREPARE_SCREEN_CALLBACK callback;
}
static ScreenModeReference[] = {
    {  640,  288, 0 },
    {  640,  432, Screen_UpscaleScreen1 },
    {  640,  576, Screen_UpscaleScreen2 },
    {  960,  576, Screen_UpscaleScreen3 },
    { 1280,  864, Screen_UpscaleScreen4 },
};

void Screen_GetScreenSize(int scrmode, int* pwid, int* phei)
{
    if (scrmode < 0 || scrmode >= sizeof(ScreenModeReference) / sizeof(ScreenModeStruct))
    {
        *pwid = *phei = 0;
    }
    else
    {
        ScreenModeStruct* pinfo = ScreenModeReference + scrmode;
        *pwid = pinfo->width;
        *phei = pinfo->height;
    }
}

void Screen_UpscaleScreen(void* pImageBits, int screenMode)
{
    if (pImageBits == 0) return;

    PREPARE_SCREEN_CALLBACK callback = ScreenModeReference[screenMode].callback;
    if (callback != 0)
        callback(pImageBits);
}

// Upscale screen from height 288 to 432
void Screen_UpscaleScreen1(void* pImageBits)
{
    int ukncline = 287;
    for (int line = 431; line > 0; line--)
    {
        DWORD* pdest = ((DWORD*)pImageBits) + line * UKNC_SCREEN_WIDTH;
        if (line % 3 == 1)
        {
            BYTE* psrc1 = ((BYTE*)pImageBits) + ukncline * UKNC_SCREEN_WIDTH * 4;
            BYTE* psrc2 = ((BYTE*)pImageBits) + (ukncline + 1) * UKNC_SCREEN_WIDTH * 4;
            BYTE* pdst1 = (BYTE*)pdest;
            for (int i = 0; i < UKNC_SCREEN_WIDTH * 4; i++)
            {
                if (i % 4 == 3)
                    *pdst1 = 0;
                else
                    *pdst1 = (BYTE)((((WORD)*psrc1) + ((WORD)*psrc2)) / 2);
                psrc1++;  psrc2++;  pdst1++;
            }
        }
        else
        {
            DWORD* psrc = ((DWORD*)pImageBits) + ukncline * UKNC_SCREEN_WIDTH;
            memcpy(pdest, psrc, UKNC_SCREEN_WIDTH * 4);
            ukncline--;
        }
    }
}

// Upscale screen from height 288 to 576 with "interlaced" effect
void Screen_UpscaleScreen2(void* pImageBits)
{
    for (int ukncline = 287; ukncline >= 0; ukncline--)
    {
        DWORD* psrc = ((DWORD*)pImageBits) + ukncline * UKNC_SCREEN_WIDTH;
        DWORD* pdest = ((DWORD*)pImageBits) + (ukncline * 2) * UKNC_SCREEN_WIDTH;
        memcpy(pdest, psrc, UKNC_SCREEN_WIDTH * 4);

        pdest += UKNC_SCREEN_WIDTH;
        memset(pdest, 0, UKNC_SCREEN_WIDTH * 4);
    }
}

// Upscale screen width 640->960, height 288->576 with "interlaced" effect
void Screen_UpscaleScreen3(void* pImageBits)
{
    for (int ukncline = 287; ukncline >= 0; ukncline--)
    {
        DWORD* psrc = ((DWORD*)pImageBits) + ukncline * UKNC_SCREEN_WIDTH;
        psrc += UKNC_SCREEN_WIDTH - 1;
        DWORD* pdest = ((DWORD*)pImageBits) + (ukncline * 2) * 960;
        pdest += 960 - 1;
        for (int i = 0; i < UKNC_SCREEN_WIDTH / 2; i++)
        {
            DWORD c1 = *psrc;  psrc--;
            DWORD c2 = *psrc;  psrc--;
            DWORD c12 =
                (((c1 & 0xff) + (c2 & 0xff)) >> 1) |
                (((c1 & 0xff00) + (c2 & 0xff00)) >> 1) & 0xff00 |
                (((c1 & 0xff0000) + (c2 & 0xff0000)) >> 1) & 0xff0000;
            *pdest = c1;  pdest--;
            *pdest = c12; pdest--;
            *pdest = c2;  pdest--;
        }

        pdest += 960;
        memset(pdest, 0, 960 * 4);
    }
}

// Upscale screen width 640->1280, height 288->864 with "interlaced" effect
void Screen_UpscaleScreen4(void* pImageBits)
{
    for (int ukncline = 287; ukncline >= 0; ukncline--)
    {
        DWORD* psrc = ((DWORD*)pImageBits) + ukncline * UKNC_SCREEN_WIDTH;
        DWORD* pdest = ((DWORD*)pImageBits) + (ukncline * 3) * 1280;
        psrc += UKNC_SCREEN_WIDTH - 1;
        pdest += 1280 - 1;
        DWORD* pdest2 = pdest + 1280;
        DWORD* pdest3 = pdest2 + 1280;
        for (int i = 0; i < UKNC_SCREEN_WIDTH; i++)
        {
            DWORD color = *psrc;  psrc--;
            *pdest = color;  pdest--;
            *pdest = color;  pdest--;
            *pdest2 = color;  pdest2--;
            *pdest2 = color;  pdest2--;
            *pdest3 = 0;  pdest3--;
            *pdest3 = 0;  pdest3--;
        }
    }
}


//////////////////////////////////////////////////////////////////////
