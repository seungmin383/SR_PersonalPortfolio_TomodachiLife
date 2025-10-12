#include"TextureCom.h"

CTextureCom::CTextureCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CTextureCom::CTextureCom(const CTextureCom& rhs)
	:CComponent(rhs)
{
}

HRESULT CTextureCom::Initialize_Origin(const _tchar* pFilePath, uint32_t iNumTextures)
{
	return E_NOTIMPL;
}

HRESULT CTextureCom::Initialize_Clone(void* _pArg)
{
	return E_NOTIMPL;
}

int32_t CTextureCom::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CTextureCom::Update(const _float _fDT)
{
	return 0;
}

void CTextureCom::Update_Late(const _float _fDT)
{
}

HRESULT CTextureCom::Render()
{
	return E_NOTIMPL;
}

CTextureCom* CTextureCom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFilePath, uint32_t iNumTextures)
{
	return nullptr;
}

CObject* CTextureCom::Clone(void* pArg)
{
	return nullptr;
}

void CTextureCom::Free()
{
	CComponent::Free();1
}
