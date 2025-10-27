#include "Mii.h"

CMii::CMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CMii::CMii(const CMii& rhs)
	:CActor(rhs)
{
}

HRESULT CMii::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMii::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CMii::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CMii::Update(_float fDT)
{
	return 0;
}

void CMii::Update_Late(_float fDT)
{
}

HRESULT CMii::Render()
{
	return E_NOTIMPL;
}

HRESULT CMii::Ready_Component()
{
	return S_OK;
}

HRESULT CMii::Bind_ShaderResources()
{
	return S_OK;
}

CObject* CMii::Clone(void* pArg)
{
	return nullptr;
}

void CMii::Free()
{
	CActor::Free();
}
