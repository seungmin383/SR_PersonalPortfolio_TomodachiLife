#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private	:
	CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Origin(
		const _tchar*	pShaderFilePath,
		const D3D11_INPUT_ELEMENT_DESC* pElements,
		uint32_t iNumElements);
	virtual HRESULT Initialize_Clone(void* _pArg)override;

	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

public:
	HRESULT Begin(uint32_t iPassIndex);

private:
	ID3DX11Effect*		m_pEffect = { nullptr };
	uint32_t			m_iNumPasses = {};
	vector<ID3D11InputLayout*>m_vecInputLayouts;

public:
	static CShader* Create(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext,
		const _tchar* pShaderFilePath,
		const D3D11_INPUT_ELEMENT_DESC* pElements,
		uint32_t iNumElements
		);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END


//텍스쳐 로드 라이브러리
//모델 빌드 라이브러리 

//
// 디버그 릴리즈 둘다 세팅해주기
// /y/s
//


