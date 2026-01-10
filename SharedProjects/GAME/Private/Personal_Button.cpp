#include "Personal_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"

#include "MiiManager.h"
#include "GameDirector.h"

CPersonal_Button::CPersonal_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CPersonal_Button::CPersonal_Button(const CPersonal_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CPersonal_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPersonal_Button::Initialize_Clone(void* pArg)
{
	PERSONALBUTTON_DESC* pDesc = static_cast<PERSONALBUTTON_DESC*>(pArg);
	m_pClicked_Strgy		= pDesc->pClicked_Strgy;
	m_pClicked_Strgy_1		= pDesc->pClicked_Strgy_1;
	m_pSelected_Strgy		= pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy	= pDesc->pSelectedClick_Strgy;

	m_vFixColor = pDesc->vFixColor;
	m_vSelectedColor = static_cast<XMFLOAT4>(Colors::Turquoise);
	m_vToggleColor = static_cast<XMFLOAT4>(Colors::DarkCyan);

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CPersonal_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CPersonal_Button::Update(_float fDT)
{
	m_fToggleTimer += fDT;
	if (m_fToggleTimer > 0.5f)
		m_isToggle = TRUE;

	if (m_isToggle)
		m_fToggleHoldTimer += fDT;

	if (m_fToggleHoldTimer > 0.3f)
	{
		m_isToggle = FALSE;

		m_fToggleHoldTimer = 0.f;
		m_fToggleTimer = 0.f;
	}

	return On_Click();
}

void CPersonal_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CPersonal_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_MIX_COLOR)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPersonal_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::MII_EDITOR), TEXT("ProtoTag_Component_PersonalButton_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPersonal_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
		return E_FAIL;

	if (m_isSelected)
	{
		if (m_isToggle)
		{
			if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &m_vToggleColor, sizeof(XMFLOAT4))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &m_vSelectedColor, sizeof(XMFLOAT4))))
				return E_FAIL;
		}
	}
	else
	{
		if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &m_vFixColor, sizeof(XMFLOAT4))))
			return E_FAIL;
	}

	return S_OK;
}

int32_t CPersonal_Button::On_Click()
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
			m_pClicked_Strgy->OnClick();
			m_pClicked_Strgy_1->OnClick();

			m_isSelected = TRUE;
		}
	}

	return 0;
}

CPersonal_Button* CPersonal_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPersonal_Button* pInstance = new CPersonal_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CPersonal_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CPersonal_Button::Clone(void* pArg)
{
	CPersonal_Button* pInstance = new CPersonal_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CPersonal_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPersonal_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pClicked_Strgy_1);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


