#include "NoneBack_TextBox.h"
#include "GameInstance.h"
#include "MiiManager.h"

CNoneBack_TextBox::CNoneBack_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CNoneBack_TextBox::CNoneBack_TextBox(const CNoneBack_TextBox& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CNoneBack_TextBox::Initialize_Origin()
{
	return S_OK;
}

HRESULT CNoneBack_TextBox::Initialize_Clone(void* pArg)
{
	NONEBACKTEXTBOX_DESC* pDesc = static_cast<NONEBACKTEXTBOX_DESC*>(pArg);

	m_vFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strText);
	m_vColor = pDesc->vColor;
	m_strText = pDesc->strText;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CNoneBack_TextBox::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CNoneBack_TextBox::Update(_float fDT)
{
	return 0;
}

void CNoneBack_TextBox::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CNoneBack_TextBox::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_DEFAULT_BLENDING)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	m_vFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strText);
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strText, XMFLOAT2{ m_fX - (m_fCX / 2.f) + 10.f, m_fY - (m_fCY / 2.f) + 5.f }, XMLoadFloat4(&m_vColor));

	return S_OK;
}

HRESULT CNoneBack_TextBox::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_Temp_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNoneBack_TextBox::Bind_ShaderResources()
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

CNoneBack_TextBox* CNoneBack_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNoneBack_TextBox* pInstance = new CNoneBack_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CNoneBack_TextBox Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CNoneBack_TextBox::Clone(void* pArg)
{
	CNoneBack_TextBox* pInstance = new CNoneBack_TextBox(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CNoneBack_TextBox Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNoneBack_TextBox::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}

