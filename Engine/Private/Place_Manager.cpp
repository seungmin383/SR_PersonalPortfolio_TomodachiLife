#include "Place_Manager.h"
#include "Place.h"

CPlaceManager::CPlaceManager()
{
}


HRESULT CPlaceManager::Go_Place(uint32_t _NextPlaceID, CPlace* _NextPlace)
{
	if (nullptr == _NextPlace)
		return E_FAIL;

	m_vecPlace.push_back(_NextPlace);

	return S_OK;
}

HRESULT CPlaceManager::Go_Back()
{
	if (1 >= m_vecPlace.size())
		return E_FAIL;

	CPlace* tmp = m_vecPlace.back();
	m_vecPlace.pop_back();
	Safe_Release(tmp);

	return S_OK;
}

int32_t CPlaceManager::Update(const _float fDT)
{
	if (m_vecPlace.empty())
		return 0;

	m_vecPlace.back()->Update(fDT);

	return 0;
}

HRESULT CPlaceManager::Render()
{
	if (m_vecPlace.empty())
		return 0;

	m_vecPlace.back()->Render();

	return 0;
}

CPlaceManager* CPlaceManager::Create()
{
	return new CPlaceManager;
}

void CPlaceManager::Free()
{
	while (!m_vecPlace.empty())
	{
		m_vecPlace.back()->Free();
		m_vecPlace.pop_back();
	}

	CBase::Free();
}
