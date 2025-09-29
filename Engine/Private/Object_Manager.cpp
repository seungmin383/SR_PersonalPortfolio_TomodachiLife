#include "Object_Manager.h"
#include "Layer.h"
#include "Object.h"
#include "GameInstance.h"

CObjectManager::CObjectManager()
	:m_pGameInstace(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstace);
}


HRESULT CObjectManager::Initialize(uint32_t FinalPlace)
{
	m_iMaxIndex = FinalPlace;
	m_mapLayer = new unordered_map<uint32_t, vector<CLayer*>>[FinalPlace];
	return S_OK;
}

int32_t CObjectManager::Update_Priority(const _float fDT)
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_mapLayer[i])	//첫번째 Place의 Pair<uint32_t, list<CLayer*>>
		{
			for (auto& Layer : pair.second)	//list<CLayer*>의 CLayer
			{
				if (nullptr == Layer)
					continue;

				m_iFlag = Layer->Update_Priority(fDT);

				if (0 != m_iFlag)
				{
					MSG_BOX("ErrCode : Flag isnt 0");
					return 1;
				}
			}
		}

	}
	return 0;
} 

int32_t CObjectManager::Update(const _float fDT)
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_mapLayer[i])	//첫번째 Place의 Pair<uint32_t, list<CLayer*>>
		{
			for (auto& Layer : pair.second)	//list<CLayer*>의 CLayer
			{
				if (nullptr == Layer)
					continue;

				m_iFlag = Layer->Update(fDT);

				if (0 != m_iFlag)
				{
					MSG_BOX("ErrCode : Flag isnt 0");
					return 1;
				}
			}
		}

	}
	return 0;
}
void CObjectManager::Update_Late(const _float fDT)
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_mapLayer[i])	//첫번째 Place의 Pair<uint32_t, list<CLayer*>>
		{
			for (auto& Layer : pair.second)	//list<CLayer*>의 CLayer
			{
				if (nullptr == Layer)
					continue;

				Layer->Update_Late(fDT);
			}
		}

	}
	return;
}
HRESULT CObjectManager::Add_Object_to_Layer(uint32_t PlaceID, uint32_t LayerID, const uint32_t ObjectID, void* pArg)
{
	CObject* tmp = m_pGameInstace->Proto_Get_Object_Clone(ObjectID, pArg);

	if (nullptr == tmp)
	{
		MSG_BOX("ErrCode : Origin Object Nullptr");
		Safe_Release(tmp);
		return E_FAIL;
	}

	auto iter = m_mapLayer[PlaceID].find(LayerID);

	if (iter == m_mapLayer[PlaceID].end())	//Layer가 없다면
	{
		CLayer* pLayer = CLayer::Create();

		if (nullptr == pLayer)
		{
			MSG_BOX("ErrCode : Layer Create Failed");
			Safe_Release(tmp);
			return E_FAIL;
		}

		pLayer->Add_Object_in_Layer(ObjectID, tmp);

		auto& vec = m_mapLayer[PlaceID][LayerID];
		vec.push_back(pLayer);

		return S_OK;
	}
	else
	{
		iter->second[LayerID]->Add_Object_in_Layer(ObjectID, tmp);
		return S_OK;
	}

	return S_OK;
}

CObjectManager* CObjectManager::Create(uint32_t FinalPlace)
{
	CObjectManager* pObjectManager = new CObjectManager;
	if (FAILED(pObjectManager->Initialize(FinalPlace)))
	{
		MSG_BOX("ErrCode : ObjectManager Create Failed");
		Safe_Release(pObjectManager);
		return nullptr;
	}

	return pObjectManager;
}

void CObjectManager::Free()
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_mapLayer[i])
		{
			for (auto& Layer : pair.second)
			{
				if (nullptr == Layer)
					continue;

				Safe_Release(Layer);
			}
			pair.second.clear();
		}
		m_mapLayer[i].clear();
	}
	Safe_Delete_Array(m_mapLayer);

	Safe_Release(m_pGameInstace);


	CBase::Free();
}
