#include "Name_TextBox.h"
#include "GameInstance.h"
#include "MiiManager.h"

CName_TextBox::CName_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CName_TextBox::CName_TextBox(const CName_TextBox& rhs)
	:CUI(rhs)
{
}

HRESULT CName_TextBox::Initialize_Origin()
{
	return S_OK;
}

HRESULT CName_TextBox::Initialize_Clone(void* pArg)
{
	NAMETEXTBOX_DESC* pDesc = static_cast<NAMETEXTBOX_DESC*>(pArg);
	m_strNameText = pDesc->strName;
	m_strRoomText = to_wstring( pDesc->iRoomIndex);

	m_vNameFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strNameText);
	m_vRoomFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strRoomText);
	m_vColor = XMFLOAT4{ 0.f, 0.f, 0.f,1.f };

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CName_TextBox::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CName_TextBox::Update(_float fDT)
{
	return 0;
}

void CName_TextBox::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CName_TextBox::Render()
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


	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strRoomText + TEXT(" ȣ"), XMFLOAT2{m_fX - (m_fCX / 2.f) + 50.f, m_fY - (m_vRoomFontSize.y * 0.5f)}, XMLoadFloat4(&m_vColor));
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strNameText, XMFLOAT2{ m_fX + (m_vNameFontSize.x * 0.5f) , m_fY - (m_vNameFontSize.y * 0.5f) }, XMLoadFloat4(&m_vColor));

	return S_OK;
}

HRESULT CName_TextBox::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Component_Name_TextBox_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CName_TextBox::Bind_ShaderResources()
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

CName_TextBox* CName_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CName_TextBox* pInstance = new CName_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CName_TextBox Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CName_TextBox::Clone(void* pArg)
{
	CName_TextBox* pInstance = new CName_TextBox(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CName_TextBox Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CName_TextBox::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}

