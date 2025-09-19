#include "Object.h"
#include "GameInstance.h"

CObject::CObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CObject::Initialize()
{
	return S_OK;
}

int32_t CObject::Update(const _float _fDT)
{
	return 0;
}

void CObject::Late_Update(const _float _fDT)
{
	return;
}

HRESULT CObject::Render()
{
	return S_OK;
}

void CObject::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	CBase::Free();
}
