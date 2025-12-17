#include "TextBox.h"
#include "GameInstance.h"

CTextBox::CTextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CTextBox::CTextBox(const CTextBox& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CTextBox::Initialize_Origin()
{
	return S_OK;
}

HRESULT CTextBox::Initialize_Clone(void* pArg)
{
	TEXTBOX_DESC* pDesc = static_cast<TEXTBOX_DESC*>(pArg);
	m_strFontTag = pDesc->strFontTag;
	m_strText = pDesc->strText;
	m_iTextureIndex = pDesc->iTextureIndex;
	m_vColor = static_cast<XMFLOAT4>(pDesc->vTextColor);

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CTextBox::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CTextBox::Update(_float fDT)
{
	return 0;
}

void CTextBox::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CTextBox::Render()
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

	m_pGameInstance->FontManager_Draw_Text(m_strFontTag, m_strText, XMFLOAT2{ m_fX - (m_fCX / 2.f) + 10.f, m_fY - (m_fCY / 2.f) + 5.f}, XMLoadFloat4(&m_vColor));
	return S_OK;
}

HRESULT CTextBox::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ROOM), TEXT("ProtoTag_Component_TextBox_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTextBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CTextBox* CTextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTextBox* pInstance = new CTextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CTextBox Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CTextBox::Clone(void* pArg)
{
	CTextBox* pInstance = new CTextBox(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CTextBox Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTextBox::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}

