#include "PlayerInven_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"

CPlayerInven_Button::CPlayerInven_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CPlayerInven_Button::CPlayerInven_Button(const CPlayerInven_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CPlayerInven_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPlayerInven_Button::Initialize_Clone(void* pArg)
{
	PLAYERINVENBUTTON_DESC* pDesc = static_cast<PLAYERINVENBUTTON_DESC*>(pArg);
	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	m_eItemType = pDesc->eItemType;
	
	m_strLeftText = pDesc->strLeftText;
	m_strRightText = pDesc->strRightText;

	m_vLeftTextSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_16"), m_strLeftText);
	m_vRightTextSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_16"), m_strRightText);

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CPlayerInven_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CPlayerInven_Button::Update(_float fDT)
{
	return On_Click();
}

void CPlayerInven_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CPlayerInven_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_16"), m_strLeftText,
		XMFLOAT2{ m_fX - (m_fCX * 0.5f) + 10.f , m_fY - (m_vLeftTextSize.y * 0.5f) }, Colors::Black);

	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_16"), m_strRightText,
		XMFLOAT2{ m_fX + (m_fCX * 0.5f) - (m_vRightTextSize.x) - 10.f, m_fY - (m_vRightTextSize.y * 0.5f)  }, Colors::Black);

	return S_OK;
}

HRESULT CPlayerInven_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_PlayerInventory_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerInven_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (m_isSelected)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture",  1);
	}
	else if (!m_isSelected)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0);
	}

	return S_OK;;
}

int32_t CPlayerInven_Button::On_Click()
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
			m_isSelected = TRUE;
		}
	}

	return 0;
}

CPlayerInven_Button* CPlayerInven_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerInven_Button* pInstance = new CPlayerInven_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CPlayerInven_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CPlayerInven_Button::Clone(void* pArg)
{
	CPlayerInven_Button* pInstance = new CPlayerInven_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CPlayerInven_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayerInven_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


