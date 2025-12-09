#include "Window_Button.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"
#include "WorldCamera.h"


CWindow_Button::CWindow_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CWindow_Button::CWindow_Button(const CWindow_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CWindow_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CWindow_Button::Initialize_Clone(void* pArg)
{
	WINDOWBUTTON_DESC* tWindowButtonDesc = static_cast<WINDOWBUTTON_DESC*>(pArg);

	m_pClicked_Strgy = tWindowButtonDesc->pClicked_Strgy;
	m_pSelected_Strgy = tWindowButtonDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = tWindowButtonDesc->pSelectedClick_Strgy;

	m_iOutlineTextureIndex = tWindowButtonDesc->iOutlineTextureIndex;

	m_vTargetPos = tWindowButtonDesc->vTargetPos;
	m_fLerpTime = tWindowButtonDesc->fLerpTime;
	m_fTargetFov = tWindowButtonDesc->fTargetFov;


	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_iTextureIndex = 1;

	return S_OK;
}


int32_t CWindow_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);	// 마우스랑 확인

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CWindow_Button::Update(_float fDT)
{
	return On_Click();
}

void CWindow_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CWindow_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_PLACEMOVEBUTTON)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWindow_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_WindowButton_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWindow_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	/* 잠겨있다면 */
	if (!m_isLived)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
			return E_FAIL;

		if (m_isSelected)
		{
		}
		else
		{
		}
	}

	return S_OK;
}

int32_t CWindow_Button::On_Click()
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

	return UPDATE_CLEAR;
}


CWindow_Button* CWindow_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWindow_Button* tmp = new CWindow_Button(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CWindow_Button::Clone(void* pArg)
{
	CWindow_Button* tmp = new CWindow_Button(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CWindow_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


