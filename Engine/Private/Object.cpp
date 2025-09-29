#include "Object.h"
#include "GameInstance.h"

CObject::CObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CObject::CObject(const CObject& rhs)
	:m_pGameInstance(rhs.m_pGameInstance)
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObject::Initialize_Origin()
{
	return S_OK;
}

HRESULT CObject::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CObject::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CObject::Update(const _float _fDT)
{
	return 0;
}

void CObject::Update_Late(const _float _fDT)
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
