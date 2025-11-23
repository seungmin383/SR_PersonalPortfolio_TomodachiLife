#include "UIManager.h"
#include "UIGroup.h"
#include "UI.h"
#include "GameInstance.h"

CUIManager::CUIManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(pDevice) , m_pContext(pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUIManager::Initialize()
{
	return S_OK;
}

void CUIManager::UIGroup_Update_Priority(_float fDT)
{
	if (m_isPlaceChange)
	{
		Clear_UIManager(m_iNextGroupIndex);
		m_isPlaceChange = FALSE;
	}

	if (m_isGroupChange)
	{
		if (m_isLoading)
		{
			Calc_FadeForce(fDT);
			m_iCurGroupIndex = m_iNextGroupIndex;

			if (m_isLoadingFin)
			{
				m_isLoading = FALSE;
				m_isLoadingFin = FALSE;
				m_isGroupChange = FALSE;
			}
		}
		/* 업데이트 진행 */
		else
		{
			m_iCurGroupIndex = m_iNextGroupIndex;
			m_isGroupChange = FALSE;
		}
	}

	auto iter = m_UIGroups.find(m_iCurGroupIndex);

	if (iter == m_UIGroups.end())
		return;

	iter->second->UI_Update_Priority(fDT);

}

void CUIManager::UIGroup_Update(_float fDT)
{
	if (m_isPlaceChange)
	{
		Clear_UIManager(m_iNextGroupIndex);
		m_isPlaceChange = FALSE;
	}

	if (m_isGroupChange)
		return;

	auto iter = m_UIGroups.find(m_iCurGroupIndex);

	if (iter == m_UIGroups.end())
		return;

	iter->second->UI_Update(fDT);

}

void CUIManager::UIGroup_Update_Late(_float fDT)
{
	if (m_isPlaceChange)
	{
		Clear_UIManager(m_iNextGroupIndex);
		m_isPlaceChange = FALSE;
	}

	if (m_isGroupChange)
		return;

	auto iter = m_UIGroups.find(m_iCurGroupIndex);

	if (iter == m_UIGroups.end())
		return;

	iter->second->UI_Update_Late(fDT);

}

HRESULT CUIManager::UIGroup_Render()
{
	if (m_isPlaceChange)
	{
		Clear_UIManager(m_iNextGroupIndex);
		m_isPlaceChange = FALSE;
	}

	if (m_isGroupChange)
		return S_OK;

	auto iter = m_UIGroups.find(m_iCurGroupIndex);

	if (iter == m_UIGroups.end())
		return S_OK;

	iter->second->UI_Render();

	return S_OK;
}

HRESULT CUIManager::Add_UI(uint32_t iUIGroupIndex, const _wstring& strUITag, CUI* pUI)
{
	auto iter = m_UIGroups.find(iUIGroupIndex);

	if (m_UIGroups.end() == iter)
	{
		CUIGroup* pInstace = CUIGroup::Create();
		if (FAILED(pInstace->Add_UI(strUITag, pUI)))
			return E_FAIL;

		m_UIGroups.emplace(iUIGroupIndex, pInstace);
		return S_OK;
	}

	if (FAILED(iter->second->Add_UI(strUITag, pUI)))
		return E_FAIL;
	return S_OK;
}

void CUIManager::Change_UIGroup(uint32_t iNextGroupIndex, _bool isLoading)
{
	m_iNextGroupIndex = iNextGroupIndex;
	m_isGroupChange = TRUE;
	m_isLoading = isLoading;

	if (m_isLoading)
	{
		m_fFadeTimer = 0.f;
		m_fFadeForce = 0.f;
		m_isLoadingFin = FALSE;
	}
}

void CUIManager::Change_Place(uint32_t iInitGroupIndex)
{
	m_iNextGroupIndex = iInitGroupIndex;
	m_isPlaceChange = TRUE;
}

void CUIManager::Reset_UIManager(uint32_t iInitGroupIndex)
{
	m_iCurGroupIndex = iInitGroupIndex;
	m_UIGroups.emplace(iInitGroupIndex, CUIGroup::Create());
}

void CUIManager::Clear_UIManager(uint32_t iInitGroupIndex)
{
	for (auto& pGroup : m_UIGroups)
	{
		Safe_Release(pGroup.second);
	}
	m_UIGroups.clear();

	Reset_UIManager(iInitGroupIndex);
}

_float CUIManager::Calc_FadeForce(_float fDT)
{
	m_fFadeTimer += fDT;

	if (m_fFadeTimer > 1.f)
	{
		m_fFadeTimer = 1.f;
		m_isLoadingFin = TRUE;
		return S_OK;
	}

	if (m_fFadeTimer < 0.5f)
	{
		m_fFadeForce = m_fFadeTimer / 0.5f;
	}
	else
	{
		_float t = (m_fFadeTimer - 0.5f) / 0.5f;
		m_fFadeForce = 1.f - t;                 
	}

	return S_OK;
}

_float CUIManager::Get_FadeForce()
{
	return m_fFadeForce;
}

CUIManager* CUIManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIManager* pInstance = new CUIManager(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CUIManager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

void CUIManager::Free()
{
	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& pGroup : m_UIGroups)
	{
		Safe_Release(pGroup.second);
	}
	m_UIGroups.clear();

	CBase::Free();
}



