#include "Layer.h"
#include "Object.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_Object(CObject* Object)
{
	m_ObjectList.push_back(Object);
	return S_OK;
}

int32_t CLayer::Update_Priority(_float fDT)
{
	for (auto& Object : m_ObjectList)
		m_iFlag = Object->Update_Priority(fDT);

	return 0;
}

int32_t CLayer::Update(_float fDT)
{
	for (auto& Object : m_ObjectList)
		m_iFlag = Object->Update(fDT);

	return 0;
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
