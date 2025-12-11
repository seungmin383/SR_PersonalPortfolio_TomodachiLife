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

	typedef struct tagKeyFrame
	{
		XMFLOAT3 vScale;
		XMFLOAT4 vRotation;
		XMFLOAT3 vTranslation;
		/* 이 키프레임이 애니메이션의 어디에 위치하는가*/
		float fTrackPosition;

	}KEYFRAME;

	typedef struct tagCutFrame
	{
		XMFLOAT4 vPosition;
		XMFLOAT3 vLook;
		_wstring strAnimationTag;

		float fTrackPosition;

	}CUTFRAME;

	typedef struct tagChannelPos
	{
		uint32_t iBoneIndex;
		XMFLOAT3 vCurScale;
		XMFLOAT4 vCurRotation;
		XMFLOAT3 vCurTranslation;
	}CHANNEL_POS;

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

	typedef struct tagVertexPos
	{
		XMFLOAT3 vPos;
		static const unsigned int iNumElements = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTX_POS;

	typedef struct tagVertexTex
	{
		XMFLOAT3 vPos;		
		XMFLOAT2 vTexCoord;
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
		XMFLOAT3 vTangent;
		XMFLOAT3 vBinormal;
		XMFLOAT2 vTexCoord;

		static const unsigned int						iNumElements = { 5 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTX_TEX_MESH;

	typedef struct tagVertexAnimMesh
	{
		XMFLOAT3 vPos;
		XMFLOAT3 vNormal;
		XMFLOAT3 vTangent;
		XMFLOAT3 vBinormal;
		XMFLOAT2 vTexCoord;
		XMUINT4 vBlendIndex;
		XMFLOAT4 vBlendWeight;

		static const unsigned int						iNumElements = { 7 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		
		};
	}VTX_TEX_ANIM_MESH;

	typedef struct tagVertexPosTexParticle
	{
		static const unsigned int		iNumElements = { 7 };

		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 5, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};
	}VTX_POS_TEX_PARTICLE;

	typedef struct tagVertexPosParticle
	{
		static const unsigned int		iNumElements = { 6 };

		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};
	}VTX_POS_PARTICLE;

	typedef struct tagVertexInstance
	{
		XMFLOAT4 vRight;
		XMFLOAT4 vUp;
		XMFLOAT4 vLook;
		XMFLOAT4 vTranslation;
		XMFLOAT2 vLifeTime;

	}VTX_PARTICLE_INST;
}


#endif // Engine_Struct_h__
