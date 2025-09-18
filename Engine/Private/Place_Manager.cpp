#include "Place_Manager.h"
#include "Place.h"

CPlaceManager::CPlaceManager()
{
}

HRESULT CPlaceManager::Go_Place(uint32_t _iPlaceName, CPlace* _CPlace)
{
	m_PlaceStack.push(_CPlace);

	return S_OK;
}

int32_t CPlaceManager::Update_Place(_float fTimeDelta)
{
	return 0;
}

CPlaceManager* CPlaceManager::Create()
{
	return new CPlaceManager;
}

void CPlaceManager::Free()
{
	CBase::Free();
}
