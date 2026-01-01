#include "Narration_TextBox.h"
#include "GameInstance.h"
#include "NarrationManager.h"

CNarration_TextBox::CNarration_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CNarration_TextBox::CNarration_TextBox(const CNarration_TextBox& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CNarration_TextBox::Initialize_Origin()
{
	return S_OK;
}

HRESULT CNarration_TextBox::Initialize_Clone(void* pArg)
{
	NARRATION_TEXTBOX_DESC* pDesc = static_cast<NARRATION_TEXTBOX_DESC*>(pArg);

	m_vFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strText);

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CNarration_TextBox::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CNarration_TextBox::Update(_float fDT)
{
	return 0;
}

void CNarration_TextBox::Update_Late(_float fDT)
{
	if (!CNarrationManager::GetInstance()->Get_isEnabledNarration())
		return;
		
	CUI::Update_Late(fDT);
}

HRESULT CNarration_TextBox::Render()
{
	if (!CNarrationManager::GetInstance()->Get_isEnabledNarration())
		return S_OK;

	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_DEFAULT_BLENDING)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;
	m_vColor = CNarrationManager::GetInstance()->Get_NarrationColor();
	m_strText = CNarrationManager::GetInstance()->Get_NarrationText();
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strText,
		XMFLOAT2{ m_fX - (m_fCX * 0.5f) + 10.f , m_fY - (m_fCY * 0.5f) + 10.f}, XMLoadFloat4(&m_vColor));

	return S_OK;
}

HRESULT CNarration_TextBox::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_Narration_TextBox_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNarration_TextBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CNarration_TextBox* CNarration_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNarration_TextBox* pInstance = new CNarration_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CNarration_TextBox Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CNarration_TextBox::Clone(void* pArg)
{
	CNarration_TextBox* pInstance = new CNarration_TextBox(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CNarration_TextBox Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNarration_TextBox::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}

