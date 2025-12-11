#include "CityhallMenu_Button.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"
#include "WorldCamera.h"


CCityhallMenu_Button::CCityhallMenu_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CCityhallMenu_Button::CCityhallMenu_Button(const CCityhallMenu_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CCityhallMenu_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CCityhallMenu_Button::Initialize_Clone(void* pArg)
{
	CITYHALLMENUBUTTON_DESC* tDesc = static_cast<CITYHALLMENUBUTTON_DESC*>(pArg);

	m_pClicked_Strgy = tDesc->pClicked_Strgy;
	m_pSelected_Strgy = tDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = tDesc->pSelectedClick_Strgy;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CCityhallMenu_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CCityhallMenu_Button::Update(_float fDT)
{
	return On_Click();
}

void CCityhallMenu_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CCityhallMenu_Button::Render()
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

HRESULT CCityhallMenu_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_Component_CityhallMenu_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCityhallMenu_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_FixColor", &Colors::Turquoise, sizeof(Colors::Turquoise))))
			return E_FAIL;
	}
	else if (m_isClicked)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 1)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	}

	return S_OK;
}

int32_t CCityhallMenu_Button::On_Click()
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

CCityhallMenu_Button* CCityhallMenu_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCityhallMenu_Button* tmp = new CCityhallMenu_Button(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CCityhallMenu_Button::Clone(void* pArg)
{
	CCityhallMenu_Button* tmp = new CCityhallMenu_Button(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CCityhallMenu_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


