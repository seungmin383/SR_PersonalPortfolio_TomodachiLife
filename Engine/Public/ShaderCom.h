#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShaderCom final : public CComponent
{
private	:
	CShaderCom(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CShaderCom(const CShaderCom& rhs);
	virtual ~CShaderCom() = default;

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

	//변수 이름으로 상수 버퍼 영`역을 찾아서,
	//지정한 메모리 블록을 그대로 바인딩하는 역할
	HRESULT Bind_RawValue(
		const _char* pConstantName,
		const void* pData,
		uint32_t iSize);

	HRESULT Bind_Matrix(
		const _char* pConstantName,
		const _float4x4* pMatrix);

	HRESULT Bind_Matrices(
		const _char* pConstantName,
		const _float4x4* pMatrix,
		uint32_t iNumMatrices);

	HRESULT Bind_ShaderResource(
		const _char* pConstantName,
		ID3D11ShaderResourceView* pSRV);

	HRESULT Bind_ShaderResources(
		const _char* pConstantName,
		ID3D11ShaderResourceView** ppSRVs,
		uint32_t iNumSRVs);

private:
	ID3DX11Effect*		m_pEffect = { nullptr };
	uint32_t			m_iNumPasses = {};
	vector<ID3D11InputLayout*>m_vecInputLayouts;

	uint32_t m_iNumViewport = {};
	D3D11_VIEWPORT m_Vieworts[2] = {};

public:
	static CShaderCom* Create(
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



