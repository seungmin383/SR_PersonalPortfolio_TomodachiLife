#include "GameDirector.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CGameDirector)

CGameDirector::CGameDirector()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

void CGameDirector::Free()
{
	Safe_Release(m_pGameInstance);
	CBase::Free();
}
