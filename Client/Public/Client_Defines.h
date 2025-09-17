#pragma once
#include <Windows.h>   // HWND, HINSTANCE 선언을 보장

extern HWND g_hWnd;
extern HINSTANCE g_Inst;

namespace Client
{

#define FPS_30 1.f / 30.f
#define FPS_60 1.f / 60.f
#define FPS_144 1.f / 144.f

#define WINCX 1280
#define WINCY 720

}