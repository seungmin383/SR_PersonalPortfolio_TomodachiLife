#pragma once
#include "Component.h"


// GitHub에서 DirectXTK
// -> DirectXTex라는 라이브러리가 따로 있음
// 대신 툴킷을 이용하면 
// 파일을 내보내기, 
// 폰트를 찍어주는 기능, 
// 충돌체를 그려주는 기능 등을 제공해준다.
// 
// 툴킷은 TGA 파일을 불러올 수 없다
// mipmap -> 로드한 이미지를 128 128만 로드하는게 아니라
// 64 64 32 32 16 16 4 4 2 2 1 1을 로드해서 들고있는다.
// 
// DX11은 시키지 않으면 안만들어서 텍스쳐 자체가 mipmap을 가지고 있어야됨
// 렌더링 라이브러리만 이런 개념을 가지고 있다.
// 
// 윈도우즈용 텍스쳐는 
// dds를 사용해야만 mipmap을 가질 수 잇다.
// 
// 다 dds를 쓰면 프레임이 굉장히 좋아진다.
// -> tga 따위는 버려도 된다.
// 쉐이더에 텍스쳐 던지는 
// 

NS_BEGIN(Engine)

class CShaderCom;

class ENGINE_DLL CTextureCom final : public CComponent
{
private:
	CTextureCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTextureCom(const CTextureCom& rhs);
	virtual ~CTextureCom() = default;

public:
	virtual HRESULT Initialize_Origin(const _tchar* pTextureFilePath, uint32_t iNumTextures);
	virtual HRESULT Initialize_Clone(void* _pArg)override;
	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

public:
	HRESULT Bind_ShaderResource(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iTextureIndex);

private:
	uint32_t m_iNumTextures = {};

	//쉐이더에 던져서 픽셀값을 가져올꺼기 때문에
	// ID3D11Texutre2D에서 파생된
	vector<ID3D11ShaderResourceView*> m_vecTexture;

public:
	static CTextureCom* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, 
		const _tchar* pTextureFilePath,
		uint32_t iNumTextures);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END          