#include "PartObject.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

HRESULT CPartObject::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPartObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CPartObject::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CPartObject::Update(_float fDT)
{
	return 0;
}

void CPartObject::Update_Late(_float fDT)
{
}

HRESULT CPartObject::Render()
{
	return S_OK;
}

void CPartObject::Free()
{
	CActor::Free();


}
