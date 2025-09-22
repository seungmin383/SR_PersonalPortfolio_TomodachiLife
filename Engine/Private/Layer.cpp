#include "Layer.h"
#include "Object.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_Object_in_Layer(const uint32_t _ObjectID, CObject* _Object)
{
	auto iter = m_mapObject.find(_ObjectID);

	if (iter == m_mapObject.end())
	{
		auto& vec = m_mapObject[_ObjectID]; // 없으면 빈 vector 생성
		vec.push_back(_Object);
		return S_OK;
	}

	for (auto& Obejct : iter->second)
	{
		if (Obejct == _Object)
		{
			MSG_BOX("ErrCode : Same Address Already There");
			return E_FAIL;
		}
	}

	iter->second.push_back(_Object);
	return S_OK;
}

int32_t CLayer::Update_Priority(const _float fDT)
{
	for (auto& pair : m_mapObject)
	{
		for (auto& Layer : pair.second)
		{
			if (nullptr == Layer)
				continue;

			m_iFlag = Layer->Update_Priority(fDT);

			if (0 != m_iFlag)
				MSG_BOX("ErrCode : Flag isnt 0");
		}
	}
	return 0;
}

int32_t CLayer::Update(const _float fDT)
{
	for (auto& pair : m_mapObject)
	{
		for (auto& Layer : pair.second)
		{
			if (nullptr == Layer)
				continue;

			m_iFlag = Layer->Update(fDT);

			if (0 != m_iFlag)
				MSG_BOX("ErrCode : Flag isnt 0");
		}
	}
	return 0;
}
void CLayer::Update_Late(const _float fDT)
{
	for (auto& pair : m_mapObject)
	{
		for (auto& Layer : pair.second)
		{
			if (nullptr == Layer)
				continue;

			Layer->Update_Late(fDT);
		}
	}
	return;
}

CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pair : m_mapObject)
	{
		for (auto& Object : pair.second) //pair.second -> vector<CObject*>
		{
			if (nullptr == Object)
				continue;

			Safe_Release(Object);
		}
		pair.second.clear();
	}
	m_mapObject.clear();

	CBase::Free();
}
