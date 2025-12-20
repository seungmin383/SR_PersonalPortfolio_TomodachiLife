#include "Save_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"
#include "MiiManager.h"
#include "ApartManager.h"

CSave_Button::CSave_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CSave_Button::CSave_Button(const CSave_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CSave_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSave_Button::Initialize_Clone(void* pArg)
{
	SAVEBUTTON_DESC* pDesc = static_cast<SAVEBUTTON_DESC*>(pArg);
	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	m_vFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), TEXT("저장"));

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CSave_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CSave_Button::Update(_float fDT)
{
	return On_Click();
}

void CSave_Button::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CSave_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		if (FAILED(m_pShader->Begin(VTS_MIX_COLOR)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShader->Begin(VTS_DEFAULT)))
			return E_FAIL;
	}

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), TEXT("저장"), XMFLOAT2{m_fX - (m_vFontSize.x * 0.5f), m_fY + (m_fCY * 0.5f) - (m_vFontSize.y) - 5.f}, Colors::Black);
	return S_OK;
}

HRESULT CSave_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Save_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSave_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
			return E_FAIL;
		if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &Colors::Turquoise, sizeof(Colors::Turquoise))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
			return E_FAIL;
	}
	return S_OK;
}

int32_t CSave_Button::On_Click()
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
			CApartManager::GetInstance()->Save_ApartData();
			m_pClicked_Strgy->OnClick();
			m_isSelected = TRUE;
		}
	}

	return 0;
}

CSave_Button* CSave_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSave_Button* pInstance = new CSave_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSave_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CSave_Button::Clone(void* pArg)
{
	CSave_Button* pInstance = new CSave_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSave_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSave_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


