#include "Category_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"

CCategory_Button::CCategory_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CCategory_Button::CCategory_Button(const CCategory_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CCategory_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CCategory_Button::Initialize_Clone(void* pArg)
{
	CATEGORYBUTTON_DESC* pDesc = static_cast<CATEGORYBUTTON_DESC*>(pArg);
	m_pClicked_Strgy		= pDesc->pClicked_Strgy;

	if(m_pClicked_Strgy_1 == nullptr)
		m_pClicked_Strgy_1		= CNull_Strgy::Create(m_pDevice, m_pContext);
	else
		m_pClicked_Strgy_1		= pDesc->pClicked_Strgy_1;

	m_pSelected_Strgy		= pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy	= pDesc->pSelectedClick_Strgy;

	m_strText = pDesc->strText;

	m_isSpecialMove = pDesc->isSpecialMove;

	m_fTargetFadeTime = pDesc->TargetFadeTime;
	m_strSoundTag = pDesc->strSoundTag;


	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fOriginX = m_fX;
	m_fOriginY = m_fY;	

	return S_OK;
}


int32_t CCategory_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	if(m_isSpecialMove)
		m_isEnable = m_pGameInstance->UIManager_Get_isEnable();

	return UPDATE_CLEAR;
}

int32_t CCategory_Button::Update(_float fDT)
{

	if (m_isSpecialMove)
	{
		if (m_isEnable)
		{
			if (!m_isFin_Fade)
				Fade_Enterance(fDT);

		}
		else
		{
			if (m_isFin_Fade)
				Fade_Exit(fDT);
		}
	}
	return On_Click();
}

void CCategory_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CCategory_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if(m_isSpecialMove)
	{
		if (FAILED(m_pShader->Begin(VTS_ALPHA_FADE)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShader->Begin(VTS_DEFAULT)))
			return E_FAIL;
	}

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	if (m_isSpecialMove)
	{
		m_vFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strShowText);
		m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strShowText, XMFLOAT2{ m_fX - (m_vFontSize.x * 0.5f), m_fY - (m_vFontSize.y * 0.5f)}, Colors::Black);
	}
	else
	{
		m_vFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strText);
		m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strText, XMFLOAT2{ m_fX - (m_vFontSize.x * 0.5f), m_fY - (m_vFontSize.y * 0.5f) }, Colors::Black);
	}
	return S_OK;
}

HRESULT CCategory_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_Category_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCategory_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (m_isSelected)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 1);
	}
	else if (!m_isSelected)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0);
	}

	if (m_isSpecialMove)
		if (FAILED(m_pShader->Bind_RawValue("g_Ratio", &m_fRatio, sizeof(_float))))
			return E_FAIL;
	return S_OK;
}

int32_t CCategory_Button::On_Click()
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
			m_pGameInstance->SoundManager_Begin_Sound(m_strSoundTag, SOUND_CHANNEL::SFX_BUTTON, 1.f);

			m_pClicked_Strgy->OnClick();
			m_pClicked_Strgy_1->OnClick();
			m_isSelected = TRUE;
		}
	}

	return 0;
}

void CCategory_Button::Fade_Enterance(_float fDT)
{
	if (m_fFadeTimer == 0.f)
	{
		if (m_strTmpText != L"")
			m_strShowText = m_strTmpText;
		else
			m_strShowText = m_strText;

		m_vTargetPos.x = m_fOriginX;
		m_vTargetPos.y = m_fOriginY;

		m_vInitPos.x = m_fOriginX;
		m_vInitPos.y = m_fOriginY + 100.f;
	}

	m_fFadeTimer += fDT;

	m_fRatio = m_fFadeTimer / m_fTargetFadeTime;

	if (m_fRatio > 1.f)
	{
		m_isFin_Fade = TRUE;
		m_fFadeTimer = m_fTargetFadeTime;
		m_fRatio = 1.f;
	}

	m_fX = m_vInitPos.x + (m_vTargetPos.x - m_vInitPos.x) * m_fRatio;
	m_fY = m_vInitPos.y + (m_vTargetPos.y - m_vInitPos.y) * m_fRatio;

	uint32_t iNumViewPorts{ 1 };
	D3D11_VIEWPORT ViewportDescs;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDescs);

	m_pTransformCom->Set_State(TRANSFORM_STATE::POS,
		XMVectorSet(m_fX - ViewportDescs.Width * 0.5f, -m_fY + ViewportDescs.Height * 0.5f, m_fZ, 1.f));

}

void CCategory_Button::Fade_Exit(_float fDT)
{

	if (m_fFadeTimer == m_fTargetFadeTime)
	{
		m_vTargetPos.x = m_fOriginX;
		m_vTargetPos.y = m_fOriginY - 100.f;

		m_vInitPos.x = m_fOriginX;
		m_vInitPos.y = m_fOriginY;
	}

	m_fFadeTimer -= fDT;

	m_fRatio = m_fFadeTimer / m_fTargetFadeTime;

	if (m_fRatio <= 0.f)
	{
		m_strTmpText = m_strText;
		m_strShowText = L"";

		m_isFin_Fade = FALSE;
		m_fFadeTimer = 0.f;
		m_fRatio = 0.f;
	}

	m_fX = m_vInitPos.x + (m_vTargetPos.x - m_vInitPos.x) * (1.f - m_fRatio);
	m_fY = m_vInitPos.y + (m_vTargetPos.y - m_vInitPos.y) * (1.f - m_fRatio);

	uint32_t iNumViewPorts{ 1 };
	D3D11_VIEWPORT ViewportDescs;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDescs);

	m_pTransformCom->Set_State(TRANSFORM_STATE::POS,
		XMVectorSet(m_fX - ViewportDescs.Width * 0.5f, -m_fY + ViewportDescs.Height * 0.5f, m_fZ, 1.f));
}

CCategory_Button* CCategory_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCategory_Button* pInstance = new CCategory_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CCategory_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CCategory_Button::Clone(void* pArg)
{
	CCategory_Button* pInstance = new CCategory_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CCategory_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCategory_Button::Free()
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


