#include "MiiHead.h"

CMiiHead::CMiiHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiHead::CMiiHead(const CMiiHead& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiHead::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiHead::Initialize_Clone(void* pArg)
{
	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CMiiHead::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CMiiHead::Update(_float fDT)
{
	return 0;
}

void CMiiHead::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
}

HRESULT CMiiHead::Render()
{
	return S_OK;
}

CMiiHead* CMiiHead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiHead* pInstance = new CMiiHead(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiHead Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiHead::Clone(void* pArg)
{
	CMiiHead* pInstance = new CMiiHead(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiHead Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiHead::Free()
{
	CPartObject::Free();
}
