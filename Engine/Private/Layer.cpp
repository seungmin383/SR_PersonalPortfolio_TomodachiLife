#include "Layer.h"
#include "Object.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_Object(CObject* pObject)
{
	m_ObjectList.push_back(pObject);
	return S_OK;
}

int32_t CLayer::Update_Priority(_float fDT)
{
	for (auto& Object : m_ObjectList)
	{
		int32_t iFlag = Object->Update_Priority(fDT);
		if (iFlag == PLACE_CHANGE)
			return iFlag;
	}

	return 0;
}

int32_t CLayer::Update(_float fDT)
{
	for (auto& Object : m_ObjectList)
	{
		int32_t iFlag = Object->Update(fDT);
		if (iFlag == PLACE_CHANGE)
			return iFlag;
	}

	return UPDATE_OK;
}
void CLayer::Update_Late(_float fDT)
{
	for (auto& Object : m_ObjectList)
		Object->Update_Late(fDT);
}

CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& Object : m_ObjectList)
		Safe_Release(Object);

	m_ObjectList.clear();

	CBase::Free();
}
