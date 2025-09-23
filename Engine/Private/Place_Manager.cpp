#include "Place_Manager.h"
#include "Place.h"

CPlaceManager::CPlaceManager()
{
}


HRESULT CPlaceManager::Go_Place(uint32_t _NextPlaceID, CPlace* _NextPlace)
{
	if (nullptr == _NextPlace)
		return E_FAIL;

	m_StackPlace.push_back(_NextPlace);

	return S_OK;
}

HRESULT CPlaceManager::Go_Back()
{
	if (1 >= m_StackPlace.size())
		return E_FAIL;

	CPlace* tmp = m_StackPlace.back();
	m_StackPlace.pop_back();
	Safe_Release(tmp);

	return S_OK;
}

int32_t CPlaceManager::Update(const _float fDT)
{
	if (m_StackPlace.empty())
		return 0;

	m_StackPlace.back()->Update(fDT);

	return 0;
}

CPlaceManager* CPlaceManager::Create()
{
	return new CPlaceManager;
}

void CPlaceManager::Free()
{
	while (!m_StackPlace.empty())
		m_StackPlace.pop_back();

	CBase::Free();
}
