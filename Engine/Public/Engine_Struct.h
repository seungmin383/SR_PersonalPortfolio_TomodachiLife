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

	typedef struct tagVertexCol
	{
		XMFLOAT3 Pos;		//위치
		XMFLOAT4 Color;		//색상

	}VERTEX_COLOR;

	/*D3D11_INPUT_ELEMENT_DESC Col_Desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/

	typedef struct tagVertexTex
	{
		XMFLOAT3 Pos;		//위치
		XMFLOAT3 Normal;	//법선
		XMFLOAT2 Tex0;		//텍스쳐 좌표 
		XMFLOAT2 Tex1;		//텍스쳐 좌표 

	}VERTEX_TEXTURE;

	/*D3D11_INPUT_ELEMENT_DESC Tex_Desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/
}


#endif // Engine_Struct_h__
