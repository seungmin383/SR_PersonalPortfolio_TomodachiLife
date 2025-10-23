#pragma once
#include <Windows.h>   // HWND, HINSTANCE 선언을 보장

#ifdef new
#undef new
#endif

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

namespace Client
{

#define FPS_30 1.f / 30.f
#define FPS_60 1.f / 60.f
#define FPS_144 1.f / 144.f

#define WINCX 960		
#define WINCY 960

	enum class PLACE_ID {
		STATIC,
		MAIN,				

		LOADING,			
		SETTING,			
		SAVE,					
		MII_EDITOR,			

		STORE_CLOTHES,		
		STORE_ANTIQUE,
		STORE_HAT,
		STORE_FOOD,
		STORE_LUXURY,
		STORE_HOUSE,		

		ENVI_FOUNTAIN,
		ENVI_OCEAN,
		ENVI_PARK,
		ENVI_PLAYGROUND,
		ENVI_CAFE,			

		ETC_COMMUINTY,
		ETC_MUSIC,
		ETC_RANKING,
		ETC_NEWS,
		ETC_PHOTO,			
			
		CONNECT_TENT,
		CONNECT_PORT,

		LIVE_APARTMENT,
		LIVE_WEDDING_HOUSE,

		END					
	};
}

namespace Maptool
{

}

using namespace std;
using namespace Client;
using namespace Maptool;