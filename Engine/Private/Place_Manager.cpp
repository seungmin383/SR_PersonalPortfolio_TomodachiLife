#include "Place_Manager.h"
#include "Place.h"
#include "GameInstance.h"

CPlaceManager::CPlaceManager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPlaceManager::Change_Place(
	uint32_t PlaceID, 
	CPlace* pPlace)
{
	if(nullptr != m_pPlace)
		m_pGameInstance->Clear_Resources(m_iPlaceID);

	m_iPlaceID = PlaceID;
	Safe_Release(m_pPlace);
	m_pPlace = pPlace;
	return S_OK;
}

int32_t CPlaceManager::Update(_float fDT)
{
	return m_pPlace->Update(fDT);
}

HRESULT CPlaceManager::Render_Priority()
{
	return m_pPlace->Render_Priority();
}

HRESULT CPlaceManager::Render_Late()
{
	return m_pPlace->Render_Late();
}

CPlaceManager* CPlaceManager::Create()
{
	return new CPlaceManager;
}

void CPlaceManager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pPlace);

	CBase::Free();
}
