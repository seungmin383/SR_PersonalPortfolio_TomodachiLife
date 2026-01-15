#include "Back_Button.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"
#include "WorldCamera.h"
#include "GameDirector.h"

#include "NarrationManager.h"

CBack_Button::CBack_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CBack_Button::CBack_Button(const CBack_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CBack_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CBack_Button::Initialize_Clone(void* pArg)
{
	BACKBUTTON_DESC* pDesc = static_cast<BACKBUTTON_DESC*>(pArg);

	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	m_strSoundTag = pDesc->strSoundTag;
	m_eChannel = pDesc->eChannel;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CBack_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CBack_Button::Update(_float fDT)
{
	return On_Click();
}

void CBack_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CBack_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		if (FAILED(m_pShaderCom->Begin(VTS_MIX_COLOR)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Begin(VTS_DEFAULT)))
			return E_FAIL;
	}

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBack_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_Back_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBack_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_FixColor", &Colors::Aqua, sizeof(Colors::Turquoise))))
			return E_FAIL;
	}

	return S_OK;

}

int32_t CBack_Button::On_Click()
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
			CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::NO_EVENT);
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);

			m_pGameInstance->SoundManager_Begin_Sound(m_strSoundTag, SOUND_CHANNEL::NONE_STOP_SFX, 1.f);
			m_pClicked_Strgy->OnClick();
			m_isSelected = TRUE;
		}
	}

	return UPDATE_CLEAR;
}

CBack_Button* CBack_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBack_Button* tmp = new CBack_Button(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CBack_Button Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CBack_Button::Clone(void* pArg)
{
	CBack_Button* tmp = new CBack_Button(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CBack_Button Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CBack_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


