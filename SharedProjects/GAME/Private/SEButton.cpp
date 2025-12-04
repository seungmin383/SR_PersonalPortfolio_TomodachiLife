#include "SEButton.h"
#include "IClick_Strgy.h"
#include "ISelect_Strgy.h"
#include "ISelectedClick_Strgy.h"
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

	m_pClick_Strgy			= pDesc->pClick_Strgy;
	m_pSelect_Strgy			= pDesc->pSelect_Strgy;
	m_pSelectedClick_Strgy	= pDesc->pSelectedClick_Strgy;
	m_pRender_Strgy			= pDesc->pRender_Strgy;
	 
	m_hasSelectedEvent		= pDesc->ButtonStateDesc.hasSelectedEvent;
	m_eClickEventState		= pDesc->ButtonStateDesc.eClickEventState;
	m_eSelectEventState		= pDesc->ButtonStateDesc.eSelectEventState;

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

void CSEButton::Calc_ButtonState()
{
	if (!m_hasSelectedEvent)
	{
		m_isSelected = FALSE;

		if (m_eClickEventState == CLICK_EVENT_STATE_PRESS)
		{
			if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressStartLBT())
			{
				m_isClicked = TRUE;
			}
		}
		else if (m_eClickEventState == CLICK_EVENT_STATE_PRESSED)
		{
			if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
			{
				m_isClicked = TRUE;
			}
		}
	}
	else if (m_hasSelectedEvent)
	{
		if (!m_isSelected)
		{
			if (m_eClickEventState == CLICK_EVENT_STATE_PRESS)
			{
				if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressStartLBT())
				{
					m_isClicked = TRUE;
				}
			}
			else if (m_eClickEventState == CLICK_EVENT_STATE_PRESSED)
			{
				if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
				{
					m_isClicked = TRUE;
				}
			}
		}
		else if (m_isSelected)
		{
			if (m_eSelectEventState == CLICK_EVENT_STATE_PRESS)
			{
				if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressStartLBT())
				{
					m_isClicked = TRUE;
				}
			}
			else if (m_eSelectEventState == CLICK_EVENT_STATE_PRESSED)
			{
				if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
				{
					m_isClicked = TRUE;
				}
			}
		}
	}
}

int32_t CSEButton::On_Click()
{
	if (nullptr == m_pClick_Strgy)
		return UPDATE_CLEAR;

	return m_pClick_Strgy->OnClicked();
}

int32_t CSEButton::On_Select()
{
	if (nullptr == m_pSelect_Strgy)
		return UPDATE_CLEAR;

	return m_pSelect_Strgy->OnSelected();
}

int32_t CSEButton::On_SelectedClick()
{
	if (nullptr == m_pSelectedClick_Strgy)
		return UPDATE_CLEAR;

	return m_pSelectedClick_Strgy->OnSelectedClick();
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

	Safe_Release(m_pClick_Strgy);
	Safe_Release(m_pSelect_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);
	Safe_Release(m_pRender_Strgy);
}