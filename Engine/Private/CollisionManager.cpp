#include "CollisionManager.h"
#include "Collider.h"
#include "Engine_FlagHelper.h"

CCollisionManager::CCollisionManager()
{
}

HRESULT CCollisionManager::Initialize(uint32_t iNumPlace)
{
	m_iNumPlace = iNumPlace;
	m_Triggers = new vector<CCollider*>[m_iNumPlace];
	m_Objects = new vector<CCollider*>[m_iNumPlace];
	return S_OK;
}

int32_t CCollisionManager::Update(_float fDT)
{
	for (uint32_t i = 0; i < m_iNumPlace; ++i)
	{
		for (auto& pTrig : m_Triggers[i])
			pTrig->Set_isHit(FALSE);

		for (auto& pObj : m_Objects[i])
			pObj->Set_isHit(FALSE);
	}

	for (uint32_t i = 0; i < m_iNumPlace; ++i)
	{
		for (const auto& pTrig : m_Triggers[i])
		{
			/* 트리거가 발동해야 되는 애들 */
			int32_t iSrcMask = pTrig->Get_Mask();

			for (auto& pObj : m_Objects[i])
			{
				/* 무슨 애인지 */
				int32_t iDstLayer = pObj->Get_Layer();

				/* 트리거가 발동해야 되는 애들 중에, 무슨 애가 있는지 */
				if (Has_Any_Flag(iSrcMask, iDstLayer))
				{
					if (pTrig->Intersect(pObj))
					{
						pTrig->Set_isHit(TRUE);
						pObj->Set_isHit(TRUE);
					}
				}
			}
		}
	}

	return UPDATE_CLEAR;
}


HRESULT CCollisionManager::Add_Collider(uint32_t iPlaceID, COLLISION eCollisionType, CCollider* pCollider)
{
	if (nullptr == pCollider)
		return E_FAIL;

	Safe_AddRef(pCollider);

	switch (eCollisionType)
	{
	case Engine::COLLISION::TRIGGER:
		m_Triggers[iPlaceID].push_back(pCollider);
		break;
	case Engine::COLLISION::OBJECT:
		m_Objects[iPlaceID].push_back(pCollider);
		break;
	default:
		break;
	}

	return S_OK;
}

CCollisionManager* CCollisionManager::Create(uint32_t iNumPlace)
{
	CCollisionManager* pInstance = new CCollisionManager();
	if (FAILED(pInstance->Initialize(iNumPlace)))
	{
		MSG_BOX("ErrCode : CCollisionManager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCollisionManager::Free()
{
	for (uint32_t i = 0; i < m_iNumPlace; ++i)
	{
		for (auto& pCollider : m_Triggers[i])
			Safe_Release(pCollider);
		m_Triggers[i].clear();
	}
	Safe_Delete_Array(m_Triggers);

	for (uint32_t i = 0; i < m_iNumPlace; ++i)
	{
		for (auto& pCollider : m_Objects[i])
			Safe_Release(pCollider);
		m_Objects[i].clear();
	}
	Safe_Delete_Array(m_Objects);

	CBase::Free();
}
