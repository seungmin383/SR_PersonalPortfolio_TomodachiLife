#include "Object_Manager.h"
#include "Object.h"

CObjectManager::CObjectManager()
{
}

HRESULT CObjectManager::Add_Object(uint32_t _ObjectID, CObject* _Object)
{
	if (nullptr == _Object)
		return E_FAIL;

	m_mapObject[_ObjectID].push_back(_Object);
	return S_OK;
}

int32_t CObjectManager::Update(const _float fDT)
{
	for (auto& pair : m_mapObject)
	{
		for (auto& Object : pair.second)
		{
			if (nullptr == Object)
				break;

			m_iFlag = Object->Update(fDT);

			if (0 != m_iFlag)
				MSG_BOX("ErrCode : Flag isnt 0");
		}
	}
	return 0;
} 

int32_t CObjectManager::Late_Update(const _float fDT)
{
	for (auto& pair : m_mapObject)
	{
		for (auto& Object : pair.second)
		{
			if (nullptr == Object)
				break;

			Object->Update(fDT);
		}

	}
	return 0;
} 

CObjectManager* CObjectManager::Create()
{
	return new CObjectManager;
}

void CObjectManager::Free()
{
	CBase::Free();
}
