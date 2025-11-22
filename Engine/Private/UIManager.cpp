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
			Show_Loading(fDT);
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

HRESULT CUIManager::Ready_Component()
{
	m_pShader = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Engine_Shader_Vtx_Tex.hlsl"), VTX_TEX::Elements, VTX_TEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pTexture = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Sprite_CityHall_Icon/Loading.dds"), 1);
	if (nullptr == m_pTexture)
		return E_FAIL;

	m_pRectBuffer = CRectBufferCom::Create(m_pDevice, m_pContext);
	if (nullptr == m_pRectBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CUIManager::Bind_ShaderResources()
{
	HRESULT hr = {};

	XMFLOAT4X4 tmpMatrix = {};
	XMStoreFloat4x4(&tmpMatrix, XMMatrixIdentity());

	m_pShader->Bind_Matrix("g_WorldMatrix", &tmpMatrix);

	m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW);

	m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ);

	m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0);

	m_pShader->Bind_RawValue("g_Fade", &m_fFadeForce, sizeof(_float));

	return hr;
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

HRESULT CUIManager::Show_Loading(_float fDT)
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

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

CUIManager* CUIManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CUIManager(pDevice, pContext);
}

void CUIManager::Free()
{
	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);

	for (auto& pGroup : m_UIGroups)
	{
		Safe_Release(pGroup.second);
	}
	m_UIGroups.clear();

	CBase::Free();
}



