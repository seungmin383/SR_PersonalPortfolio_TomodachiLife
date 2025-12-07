#include "PlaceMarker_Button.h"
#include "IClick_Strgy.h"
#include "IRender_Strgy.h"
#include "Engine_FlagHelper.h"
#include "GameInstance.h"

CPlaceMarker_Button::CPlaceMarker_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

HRESULT CPlaceMarker_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPlaceMarker_Button::Initialize_Clone(void* pArg)
{
	PLACEMARKER_DESC* pDesc = static_cast<PLACEMARKER_DESC*>(pArg);

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

	m_eClickEventState = pDesc->eClickState;

	m_iTextureIndex = pDesc->iTextureIndex;
	m_iOutlineTextureIndex = pDesc->iOutlineTextureIndex;

	CButton::Initialize_Clone(pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CPlaceMarker_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	m_iReturnFlag = 0;

	Calc_ButtonState();

	return 0;
}

int32_t CPlaceMarker_Button::Update(_float fDT)
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

void CPlaceMarker_Button::Update_Late(_float fDT)
{
	CButton::Update_Late(fDT);
}

HRESULT CPlaceMarker_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_PLACEMOVEBUTTON)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPlaceMarker_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_PlaceMarker_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_PlaceMarker_Button_OutLine_Texture"),
		TEXT("Com_OutLineTexture"), reinterpret_cast<CComponent**>(&m_pOutlineTexture), nullptr)))
		return E_FAIL;
}

HRESULT CPlaceMarker_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (!m_isUnlock)
	{
		if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex + 1)))
			return E_FAIL;

		if (m_isSelected)
		{
			if (FAILED(m_pOutlineTexture->Bind_ShaderResource(m_pShader, "g_UpperTexture", m_iOutlineTextureIndex)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pOutlineTexture->Bind_ShaderResource(m_pShader, "g_UpperTexture", 5)))
				return E_FAIL;
		}
	}

	return S_OK;
}

void CPlaceMarker_Button::Calc_ButtonState()
{
	switch (m_eClickEventState)
	{
	case Game::CPlaceMarker_Button::CLICK_EVENT_STATE_PRESS:
		if (m_pGameInstance->MouseManager_Get_isPressStartLBT())
			m_isClicked = TRUE;
		break;
	case Game::CPlaceMarker_Button::CLICK_EVENT_STATE_PRESSED:
		if (m_pGameInstance->MouseManager_Get_isClickLBT())
			m_isClicked = TRUE;
		break;
	default:
		break;
	}
}

int32_t CPlaceMarker_Button::On_Click()
{
	if (!m_HasClickedEvent)
		return UPDATE_CLEAR;

	return m_pClick_Strgy[CLICKED]->OnClicked();
}

int32_t CPlaceMarker_Button::On_Select()
{
	if (!m_HasSelectedEvent)
		return UPDATE_CLEAR;

	return m_pClick_Strgy[SELECTED]->OnClicked();
}

int32_t CPlaceMarker_Button::On_SelectedClick()
{
	if (!m_HasSelectedClickEvent)
		return UPDATE_CLEAR;

	return m_pClick_Strgy[SELECTEDCLICK]->OnClicked();
}

CPlaceMarker_Button* CPlaceMarker_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlaceMarker_Button* pInstance = new CPlaceMarker_Button(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CPlaceMarker_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

inline CObject* CPlaceMarker_Button::Clone(void* pArg)
{
	CPlaceMarker_Button* pInstance = new CPlaceMarker_Button(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CPlaceMarker_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

inline void CPlaceMarker_Button::Free()
{
	CButton::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pOutlineTexture);
	Safe_Release(m_pRectBuffer);

	for (uint32_t i = 0; i < CLICKSTRGY_TYPE::END; ++i)
	{
		if (m_pClick_Strgy[i] == nullptr)
			continue;

		Safe_Release(m_pClick_Strgy[i]);
	}

}