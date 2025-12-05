#include "SEButton.h"
#include "IClick_Strgy.h"
#include "IRender_Strgy.h"
#include "Engine_FlagHelper.h"
#include "GameInstance.h"

CSEButton::CSEButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

HRESULT CSEButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSEButton::Initialize_Clone(void* pArg)
{
	SEBUTTON_DESC* pDesc	= static_cast<SEBUTTON_DESC*>(pArg);

	if (nullptr != pDesc->pClick_Strgy)
	{
		m_pClick_Strgy[CLICKED] = pDesc->pClick_Strgy;
		m_HasClickedEvent = TRUE;
	}

	if (nullptr != pDesc->pSelect_Strgy)
	{
		m_pClick_Strgy[SELECTED] = pDesc->pSelect_Strgy;
		m_HasSelectedEvent = TRUE;
	}

	if (nullptr != pDesc->pSelectedClick_Strgy)
	{
		m_pClick_Strgy[SELECTEDCLICK] = pDesc->pSelectedClick_Strgy;
		m_HasSelectedClickEvent = TRUE;
	}

	m_pRender_Strgy			= pDesc->pRender_Strgy;
	m_eClickEventState		= pDesc->eClickState;

	return S_OK;
}

int32_t CSEButton::Update_Priority(_float fDT)
{
	/* m_isClicked 초기화 및 m_isOverlapped 계산 */
	/* m_isSelected 는 필요할 때 초기화 하자 */
	CButton::Update_Priority(fDT);

	m_iReturnFlag = 0;

	Calc_ButtonState();

	return 0;
}

int32_t CSEButton::Update(_float fDT)
{
	if (m_isSelected)
	{
		if (m_isClicked)
		{
			Add_Flag(m_iReturnFlag, On_SelectedClick());
		}
		else
		{
			Add_Flag(m_iReturnFlag, On_Select());
		}
	}
	else
	{
		if (m_isClicked)
		{
			Add_Flag(m_iReturnFlag, On_Click());
			m_isSelected = TRUE;
		}
	}

	return m_iReturnFlag;
}

void CSEButton::Update_Late(_float fDT)
{
}

HRESULT CSEButton::Render()
{
	return m_pRender_Strgy->Render();
}


HRESULT CSEButton::Ready_Component()
{
	return S_OK;
}

HRESULT CSEButton::Bind_ShaderResources()
{
	return S_OK;
}

void CSEButton::Calc_ButtonState()
{
	switch (m_eClickEventState)
	{
	case Game::CSEButton::CLICK_EVENT_STATE_PRESS:
		if (m_pGameInstance->MouseManager_Get_isPressStartLBT())
			m_isClicked = TRUE;
		break;
	case Game::CSEButton::CLICK_EVENT_STATE_PRESSED:			
		if (m_pGameInstance->MouseManager_Get_isClickLBT())
			m_isClicked = TRUE;
		break;
	default:
		break;
	}
}

int32_t CSEButton::On_Click()
{
	if (!m_HasClickedEvent)
		return UPDATE_CLEAR;

	return m_pClick_Strgy[CLICKED]->OnClicked();
}

int32_t CSEButton::On_Select()
{
	if (!m_HasSelectedEvent)
		return UPDATE_CLEAR;

	return m_pClick_Strgy[SELECTED]->OnClicked();
}

int32_t CSEButton::On_SelectedClick()
{
	if (!m_HasSelectedClickEvent)
		return UPDATE_CLEAR;

	return m_pClick_Strgy[SELECTEDCLICK]->OnClicked();
}

CSEButton* CSEButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEButton* pInstance = new CSEButton(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEButton Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

inline CObject* CSEButton::Clone(void* pArg)
{
	CSEButton* pInstance = new CSEButton(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEButton Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

inline void CSEButton::Free()
{
	CButton::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);

	for (uint32_t i = 0; i < CLICKSTRGY_TYPE::END; ++i)
	{
		Safe_Release(m_pClick_Strgy[i]);
	}

	Safe_Release(m_pRender_Strgy);
}