#include "TextButton.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "MiiManager.h"

CTextButton::CTextButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CTextButton::CTextButton(const CTextButton& rhs)
	:CButton(rhs)
{
}

HRESULT CTextButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CTextButton::Initialize_Clone(void* pArg)
{
	TEXTBUTTON_DESC* pDesc = static_cast<TEXTBUTTON_DESC*>(pArg);

	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	m_eTextType = pDesc->eTextType;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CTextButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CTextButton::Update(_float fDT)
{
	return On_Click();
}

void CTextButton::Update_Late(_float fDT)
{
	m_strText = m_pGameInstance->ButtonManager_Get_Text();

	switch (m_eTextType)
	{
	case Game::CTextButton::FIRSTNAME:
		CMiiManager::GetInstance()->Get_Profile().strFirstName = WstrToStr( m_strText);
		break;
	case Game::CTextButton::LASTNAME:
		CMiiManager::GetInstance()->Get_Profile().strLastName = WstrToStr( m_strText);
		break;
	case Game::CTextButton::NICKNAME:
		CMiiManager::GetInstance()->Get_Profile().strNickName = WstrToStr( m_strText);
		break;
	default:
		break;
	}

	CUI::Update_Late(fDT);
}

HRESULT CTextButton::Render()
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

	return S_OK;
}

HRESULT CTextButton::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::MII_EDITOR), TEXT("ProtoTag_Component_TextButton_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTextButton::Bind_ShaderResources()
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

int32_t CTextButton::On_Click()
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
			MSG_BOX("이름을 입력하세요 ");
			m_pGameInstance->ButtonManager_Set_OnText(TRUE);
			m_pClicked_Strgy->OnClick();
			m_isSelected = TRUE;
		}
	}
	return UPDATE_CLEAR;
}

CTextButton* CTextButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTextButton* pInstance = new CTextButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CTextButton Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CTextButton::Clone(void* pArg)
{
	CTextButton* pInstance = new CTextButton(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CTextButton Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTextButton::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}



