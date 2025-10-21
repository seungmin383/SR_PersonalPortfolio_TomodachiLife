#include "Place_Manager.h"
#include "Place.h"
#include "GameInstance.h"

CPlaceManager::CPlaceManager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPlaceManager::Change_Place(
	RETAIN_POLICY Policy,
	uint32_t PlaceID, 
	CPlace* pPlace)
{

	//이전씬을 기억할까?
	switch (Policy)
	{
	case Engine::RETAIN_POLICY::RETAIN: //유지하겠다.
		break;

	case Engine::RETAIN_POLICY::CLEAR_LOADING:
		Safe_Release(m_pPlaces.back());
		m_pPlaces.pop_back();
		m_iPlaceID.pop_back();
		break;

	case Engine::RETAIN_POLICY::CLEAR_ALL:
		Clear_Place();
		break;

	default:
		break;
	}

	m_pPlaces.push_back(pPlace);
	m_iPlaceID.push_back(PlaceID);

	return S_OK;
}

HRESULT CPlaceManager::Return_Place()
{
	if (m_pPlaces.size() <= 1)
		return E_FAIL;

	Safe_Release(m_pPlaces.back());
	m_pGameInstance->Clear_Resources(m_iPlaceID.back());

	m_pPlaces.pop_back();
	m_iPlaceID.pop_back();

	return S_OK;
}

HRESULT CPlaceManager::Clear_Place()
{
	for (auto& place : m_pPlaces)
		Safe_Release(place);

	for(auto PlaceID : m_iPlaceID)
		m_pGameInstance->Clear_Resources(PlaceID);

	m_pPlaces.clear(); 
	m_iPlaceID.clear();

	return S_OK;
}

int32_t CPlaceManager::Update(_float fDT)
{
	if (m_pPlaces.empty())
		return 1;

	m_pPlaces.back()->Update(fDT);

	return 0;
}

HRESULT CPlaceManager::Render_Priority()
{
	if (m_pPlaces.empty())
		return E_FAIL;

	m_pPlaces.back()->Render_Priority();

	return S_OK;
}

HRESULT CPlaceManager::Render_Late()
{
	if (m_pPlaces.empty())
		return E_FAIL;

	m_pPlaces.back()->Render_Late();

	return S_OK;
}

CPlaceManager* CPlaceManager::Create()
{
	return new CPlaceManager;
}

void CPlaceManager::Free()
{
	Safe_Release(m_pGameInstance);
	
	for (auto& place : m_pPlaces)
		Safe_Release(place);
	m_pPlaces.clear();


	CBase::Free();
}
