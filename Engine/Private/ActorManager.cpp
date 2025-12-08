#include "ActorManager.h"
#include "Layer.h"
#include "Object.h"
#include "GameInstance.h"

CActorManager::CActorManager()
	:m_pGameInstace(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstace);
}


CActorManager* CActorManager::Create()
{
	return new CActorManager();
}

void CActorManager::Free()
{
	Safe_Release(m_pGameInstace);
	CBase::Free();
}
