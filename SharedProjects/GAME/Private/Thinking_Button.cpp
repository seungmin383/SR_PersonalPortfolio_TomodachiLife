#include "Thinking_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"

#include "MiiManager.h"
#include "GameDirector.h"

CThinking_Button::CThinking_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CThinking_Button::CThinking_Button(const CThinking_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CThinking_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CThinking_Button::Initialize_Clone(void* pArg)
{
	THINKINGBUTTON_DESC* pDesc = static_cast<THINKINGBUTTON_DESC*>(pArg);
	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CThinking_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	m_eCurMii_Needs = static_cast<NEEDS>(CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.iNeeds);

	return UPDATE_CLEAR;
}

int32_t CThinking_Button::Update(_float fDT)
{
	return On_Click();
}

void CThinking_Button::Update_Late(_float fDT)
{
	if (CGameDirector::GetInstance()->Get_Direction() == CGameDirector::GAME_DIRECTION::NO_EVENT)
	{
		if (m_eCurMii_Needs != NEEDS::NOTHING && m_eCurMii_Needs != NEEDS::SOLO && m_eCurMii_Needs != NEEDS::COUPLE)
		{
			CUI::Update_Late(fDT);
			m_isRender = TRUE;
			return;
		}
	}
	m_isRender = FALSE;
}

HRESULT CThinking_Button::Render()
{
	if (!m_isRender)
		return S_OK;

	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CThinking_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ROOM), TEXT("ProtoTag_Component_Thinking_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CThinking_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
		return E_FAIL;	


	return S_OK;
}

int32_t CThinking_Button::On_Click()
{
	if (m_isSelected)
	{
		m_pSelected_Strgy->OnClick();

		if (m_isClicked)
		{
			m_pSelectedClick_Strgy->OnClick();
		}
	}
	else
	{
		if (m_isClicked)
		{
			m_pGameInstance->SoundManager_Begin_Sound(TEXT("SFX_Selected"), SOUND_CHANNEL::SFX_BUTTON, 1.f);

			switch (m_eCurMii_Needs)
			{
			case Game::NEEDS::NOTHING:
				break;
			case Game::NEEDS::HUNGRY:
				CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::HUNGRY);
				break;
			case Game::NEEDS::BE_FRIEND:
				CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::BE_FRIEND);
				break;

			case Game::NEEDS::LOVE:
				CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::LOVE);
				break;

			default:
				break;
			}
			m_pClicked_Strgy->OnClick();
			m_isSelected = TRUE;
		}
	}

	return 0;
}

CThinking_Button* CThinking_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CThinking_Button* pInstance = new CThinking_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CThinking_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CThinking_Button::Clone(void* pArg)
{
	CThinking_Button* pInstance = new CThinking_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CThinking_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CThinking_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


