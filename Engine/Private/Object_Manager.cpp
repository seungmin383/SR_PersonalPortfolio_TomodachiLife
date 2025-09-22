#include "Object_Manager.h"
#include "Layer.h"
#include "Object.h"
#include "GameInstance.h"

CObjectManager::CObjectManager()
	:m_pGameInstace(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstace);
}


int32_t CObjectManager::Update_Priority(const _float fDT)
{
	for (auto& pair : m_mapLayer)
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

int32_t CObjectManager::Update(const _float fDT)
{
	for (auto& pair : m_mapLayer)
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
void CObjectManager::Update_Late(const _float fDT)
{
	for (auto& pair : m_mapLayer)
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
HRESULT CObjectManager::Add_Object_to_Layer(uint32_t _LayerID, const uint32_t _ObjectID)
{
	CObject* tmp = m_pGameInstace->Proto_Get_Object_Clone(_ObjectID);

	if (nullptr == tmp)
	{
		MSG_BOX("ErrCode : Origin Object Nullptr");
		Safe_Release(tmp);
		return E_FAIL;
	}

	auto iter = m_mapLayer.find(_LayerID);

	CLayer* src = CLayer::Create();
	
	if (nullptr == src)
	{
		MSG_BOX("ErrCode : Layer Create Failed");
		Safe_Release(tmp);
		return E_FAIL;
	}
	if (iter == m_mapLayer.end())
	{
		auto& vec = m_mapLayer[_LayerID];
		src->Add_Object_in_Layer(_ObjectID, tmp);
		vec.push_back(src);
		return S_OK;
	}
	else
	{
		iter->second[_ObjectID]->Add_Object_in_Layer(_ObjectID, tmp);
		return S_OK;
	}
}


CObjectManager* CObjectManager::Create()
{
	return new CObjectManager;
}

void CObjectManager::Free()
{
	Safe_Release(m_pGameInstace);

	CBase::Free();
}
