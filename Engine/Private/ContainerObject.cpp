#include "ContainerObject.h"

CContainerObject::CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

HRESULT CContainerObject::Initialize_Origin()
{
	return S_OK;
}

HRESULT CContainerObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CContainerObject::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CContainerObject::Update(_float fDT)
{
	return 0;
}

void CContainerObject::Update_Late(_float fDT)
{
}

HRESULT CContainerObject::Render()
{
	return S_OK;
}

void CContainerObject::Free()
{
	CActor::Free();


}
