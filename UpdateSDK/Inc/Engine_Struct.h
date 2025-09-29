#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagEngine
	{
		uint32_t PlaceCount = {};

	}ENGINE_DESC;


	typedef struct tagGraphicDeviceInit
	{
		HWND	hWnd;
		WINMODE WinMode;
		uint32_t iWinCX, iWinCY;

	}GRAPHIC_DEVICE_INIT_DESC;

	typedef struct tagVertexCol
	{
		XMFLOAT3 Pos;		//위치
		XMFLOAT4 Color;		//색상

	}VERTEX_COLOR;

	typedef struct tagVertexTex
	{
		XMFLOAT3 Pos;		//위치
		XMFLOAT3 Normal;	//법선
		XMFLOAT2 Tex0;		//텍스쳐 좌표 
		XMFLOAT2 Tex1;		//텍스쳐 좌표 

	}VERTEX_TEXTURE;

	typedef struct tagColor
	{
		XMFLOAT4 White		= { 1.f, 1.f, 1.f, 1.f };
		XMFLOAT4 Black		= { 0.f, 0.f, 0.f, 1.f };
		XMFLOAT4 Red		= { 1.f, 0.f, 0.f, 1.f };
		XMFLOAT4 Green		= { 0.f, 1.f, 0.f, 1.f };
		XMFLOAT4 Blue		= { 0.f, 0.f, 1.f, 1.f };
		XMFLOAT4 Yellow		= { 1.f, 1.f, 0.f, 1.f };
		XMFLOAT4 Cyan		= { 0.f, 1.f, 1.f, 1.f };
		XMFLOAT4 Magenta	= { 1.f, 0.f, 1.f, 1.f };
	}COLOR_DESC;
}


#endif // Engine_Struct_h__
