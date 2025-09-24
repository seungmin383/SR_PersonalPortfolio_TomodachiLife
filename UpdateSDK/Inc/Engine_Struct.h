#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagGraphicDeviceInit
	{
		HWND	hWnd;
		WINMODE WinMode;
		uint32_t iWinCX, iWinCY;

	}GRAPHIC_DEVICE_INIT_DESC;

	typedef struct tagVector3
	{
		_float x, y, z;

		tagVector3 Normalize()
		{

		}
	}VECTOR3;
}


#endif // Engine_Struct_h__
