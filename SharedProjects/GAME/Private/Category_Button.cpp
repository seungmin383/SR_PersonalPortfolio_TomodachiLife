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
	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CCategory_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CCategory_Button::Update(_float fDT)
{
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

	if (FAILED(m_pShader->Begin(VTS_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

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
			m_pClicked_Strgy->OnClick();
			m_isSelected = TRUE;
		}
	}

	return 0;
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
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


