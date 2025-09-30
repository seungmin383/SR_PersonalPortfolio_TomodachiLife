#include "Shader.h"

CShader::CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CComponent(_pDevice, _pContext)
{
}

CShader::CShader(const CShader& rhs)
	:CComponent(rhs)
{
}

HRESULT CShader::Initialize_Origin(
	const _tchar* pShaderFilePath, 
	const D3D11_INPUT_ELEMENT_DESC* pElements,
	uint32_t iNumElements)
{
	return S_OK;
}

HRESULT CShader::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CShader::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CShader::Update(const _float _fDT)
{
	return 0;
}

void CShader::Update_Late(const _float _fDT)
{
}

HRESULT CShader::Render()
{
	return S_OK;
}

HRESULT CShader::Begin(uint32_t iPassIndex)
{
	return S_OK;
}

CShader* CShader::Create(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext,
	const _tchar* pShaderFilePath,
	const D3D11_INPUT_ELEMENT_DESC* pElements,
	uint32_t iNumElements
)
{
	CShader* pShader = new CShader(_pDevice, _pContext);
	if (FAILED(pShader->Initialize_Origin(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("ErrCode : Shader Create Failed");
		Safe_Release(pShader);
	}
	return pShader;
}
CObject* CShader::Clone(void* pArg)
{
	CShader* pClone = new CShader(*this);
	if (FAILED(pClone->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Shader Clone Failed");
		Safe_Release(pClone);
	}
	return pClone;
}

void CShader::Free()
{
	CComponent::Free();
}
