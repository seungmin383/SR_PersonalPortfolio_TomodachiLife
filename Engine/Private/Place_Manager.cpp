#include "Place_Manager.h"
#include "Place.h"

CPlaceManager::CPlaceManager()
{
}

HRESULT CPlaceManager::Go_Place(CPlace* _CPlace)
{
	if (nullptr == _CPlace)
		return E_FAIL;

	m_StackPlace.push(_CPlace);

	return S_OK;
}

HRESULT CPlaceManager::Go_Back()
{
	if (1 >= m_StackPlace.size())
		return E_FAIL;

	CPlace* tmp = m_StackPlace.top();
	m_StackPlace.pop();
	Safe_Release(tmp);

	return S_OK;
}

int32_t CPlaceManager::Update(const _float fDT)
{
	if (m_StackPlace.empty())
		return 0;

	m_StackPlace.top()->Update(fDT);

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
