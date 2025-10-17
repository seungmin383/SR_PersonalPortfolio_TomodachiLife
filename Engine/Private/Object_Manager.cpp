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
	m_Layers = new map<const _wstring, CLayer*>[FinalPlace];
	return S_OK;
}

int32_t CObjectManager::Update_Priority(_float fDT)
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_Layers[i])
			pair.second->Update_Priority(fDT);
	}

	return 0;
} 

int32_t CObjectManager::Update(_float fDT)
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_Layers[i])
			pair.second->Update(fDT);
	}

	return 0;
}
void CObjectManager::Update_Late(_float fDT)
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_Layers[i])
			pair.second->Update_Late(fDT);
	}
}

HRESULT CObjectManager::Add_Object_in_Layer(
	uint32_t PlaceID,
	const _wstring LayerTag,
	CObject* pObject)
{
	if (m_iMaxIndex <= PlaceID)
		return E_FAIL;

	auto iter = m_Layers[PlaceID].find(LayerTag);

	//만약 없다 -> Layer를 만들고 Layer에 Object 추가 후 Layers에 추가

	if (iter == m_Layers[PlaceID].end())
	{
		CLayer* pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_Object(pObject);

		m_Layers[PlaceID].emplace(LayerTag, pLayer);
		return S_OK;
	}
	//만약 있다 -> 있는 곳의 Layer에 Object 추가
	else
	{
		iter->second->Add_Object(pObject);
		return S_OK;
	}
}

HRESULT CObjectManager::Clear_Objects(uint32_t PlaceID)
{
	if (PlaceID >= m_iMaxIndex)
		return E_FAIL;

	for (auto& pair : m_Layers[PlaceID])
		Safe_Release(pair.second);
	m_Layers[PlaceID].clear();

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
		for (auto& pair : m_Layers[i])
			Safe_Release(pair.second);
		m_Layers[i].clear();
	}

	Safe_Delete_Array(m_Layers);

	Safe_Release(m_pGameInstace);
	CBase::Free();
}
