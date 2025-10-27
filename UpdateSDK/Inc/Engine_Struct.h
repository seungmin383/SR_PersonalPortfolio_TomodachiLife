#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagEngine
	{
		uint32_t	PlaceCount = {}; 
		HWND		hWnd;
		HINSTANCE	hInst;
		WINMODE		WinMode;
		uint32_t	iWinCX, iWinCY;

	}ENGINE_DESC;

	typedef struct tagLight
	{
		LIGHT_TYPE eType;

		XMFLOAT4 fDiffuse;
		XMFLOAT4 fAmbient;
		XMFLOAT4 fSpecular;

		XMFLOAT4 vDir;
		XMFLOAT4 vPos;
		_float fRange;
	}LIGHT_DESC;


	typedef struct tagVertexCol
	{
		XMFLOAT3 Pos;		//위치
		XMFLOAT4 Color;		//색상
	}VERTEX_COLOR;

	/* D3D11_INPUT_ELEMENT_DESC
	LPCSTR						SemanticName;
    UINT						SemanticIndex;
    DXGI_FORMAT					Format;
    UINT						InputSlot;
    UINT						AlignedByteOffset;
    D3D11_INPUT_CLASSIFICATION	InputSlotClass;
    UINT						InstanceDataStepRate;
	*/

	typedef struct tagVertexTex
	{
		XMFLOAT3 vPos;		// 위치
		XMFLOAT2 vTexCoord;	// 텍스쳐 좌표 
		static const unsigned int						iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

	}VTX_TEX;

	typedef struct tagVertexNormal
	{
		XMFLOAT3 vPos;		// 위치
		XMFLOAT3 vNormal;	// 노말
		XMFLOAT2 vTexCoord;	// 텍스쳐 좌표 
		static const unsigned int						iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

	}VTX_TEX_NORMAL;

	typedef struct tagVertexMesh
	{
		XMFLOAT3 vPos;
		XMFLOAT3 vNormal;
		XMFLOAT2 vTexCoord;
		XMFLOAT3 vTangent;

		static const unsigned int						iNumElements = { 4 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTX_TEX_MESH;

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
