#include "MiiBody.h"
#include "GameInstance.h"

CMiiBody::CMiiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiBody::CMiiBody(const CMiiBody& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiBody::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiBody::Initialize_Clone(void* pArg)
{
	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CMiiBody::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CMiiBody::Update(_float fDT)
{
	return 0;
}

void CMiiBody::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
}

HRESULT CMiiBody::Render()
{
	return S_OK;
}

HRESULT CMiiBody::Ready_Component()
{
	return S_OK;
}

HRESULT CMiiBody::Bind_ShaderResources()
{
	return S_OK;
}


CMiiBody* CMiiBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiBody* pInstance = new CMiiBody(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiBody Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiBody::Clone(void* pArg)
{
	CMiiBody* pInstance = new CMiiBody(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiBody Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiBody::Free()
{
	CPartObject::Free();
}
