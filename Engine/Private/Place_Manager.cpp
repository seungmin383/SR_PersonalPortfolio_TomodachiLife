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
	m_isChangePlace = TRUE;
	m_iNextPlaceID = PlaceID;
	m_pNextPlace = pPlace;
	return S_OK;
}

void CPlaceManager::Go_Place()
{
	if (nullptr != m_pPlace)
		m_pGameInstance->Clear_Resources(m_iPlaceID);

	Safe_Release(m_pPlace);

	m_iPlaceID = m_iNextPlaceID;
	m_pPlace = m_pNextPlace;
	m_isChangePlace = FALSE;
}

int32_t CPlaceManager::Update(_float fDT)
{
	if (m_isChangePlace)
		Go_Place();

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
