#pragma once

#include <Windows.h>   // HWND, HINSTANCE 선언을 보장
#include "Engine_Define.h"

#include "Game_Struct.h"
#include "SEMiiInformation.h"

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

namespace Game
{

#define FPS_30 1.f / 30.f
#define FPS_60 1.f / 60.f
#define FPS_144 1.f / 144.f

#define WINCX 960		
#define WINCY 960

	/* Texture2D */
	/* g_Texture */
#define VTS_DEFAULT 0

	/* Texture2D */
	/* g_UpperTexture, g_Texture */
#define VTS_MIX_TEXTURE 1

	/* float */
	/* g_Brightness */
#define VTS_BRIGHTEN 2
	
	/* Texture2D, float4 */
	/* g_Texture, g_FixColor */
#define VTS_COLOR 3

	/* Texture2D, float4 */
	/* g_Texture, g_FixColor */
#define VTS_MIX_COLOR 4

#define VTS_PLACEMOVEBUTTON 5

	/* float */
	/* g_Pow */
#define VTS_TEXCOORD 6

#define VTS_DEFAULT_BLENDING 7

	/* float */
	/* g_TexCoord*/
#define VTS_GAUGE_RIGHT 8

	/* float */
	/* g_TexCoord*/
#define VTS_GAUGE_UP 9

	/* texture2D */
	/* g_DiffuseTexture */
#define VMS_DEFAULT 0

	/* float4 */
	/* g_BaseColor */
#define VMS_COLOR 1

	/* float4 */
	/* g_BaseColor */
#define VMS_NONE_COLOR 2

	/* texture2D, float */
	/* g_DiffuseTexture, g_Brightness */
#define VMS_BRIGHTNESS 3
#define VMS_SHADOW 4
#define VMS_LIGHT 5
#define VMS_BRIGHTNESS2 6




#define VAMS_DEFAULT 0
#define VAMS_ALPHABLEND 1
#define VAMS_SHADOW 2
#define VAMS_LIGHT 3
#define VAMS_BRIGHTNESS 4
	enum class PLACEID {
		STATIC,
		ISLAND,
		APART,
		CITYHALL,
		MII_EDITOR,
		ROOM,
		FOOD_STORE,

		DEVELOPER,
		LOADING,
		END
	};

	enum class TEXTURE_TYPE
	{
		DIFFUSE,
		MASK,
		END
	};

	enum COLLIDER_TYPES
	{
		COL_TRIGGER = 1 << 0,
		COL_OBJECTS = 1 << 1
	};

	enum class ITEM_TYPE
	{
		FOOD,
		CLOTH,
		HOUSE,
		END
	};

	enum class NEEDS
	{
		NOTHING,
		HUNGRY,
		BE_FRIEND,
		END
	};
}
using namespace std;
using namespace Game;