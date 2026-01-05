#include "Friend_TextBox.h"
#include "GameInstance.h"
#include "MiiManager.h"


CFriend_TextBox::CFriend_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CFriend_TextBox::CFriend_TextBox(const CFriend_TextBox& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CFriend_TextBox::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFriend_TextBox::Initialize_Clone(void* pArg)
{
	FRIENDTEXTBOX_DESC* pDesc = static_cast<FRIENDTEXTBOX_DESC*>(pArg);

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CFriend_TextBox::Update_Priority(_float fDT)
{
	if (!CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.iFriendIndexList.empty())
	{
		uint32_t i = CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.iFriendIndexList.front();
		_wstring	str = StrToWstr( CMiiManager::GetInstance()->Get_MiiInfo(i).MiiProfile.strNickName);

		m_strText = str;
	}

	return 0;
}

int32_t CFriend_TextBox::Update(_float fDT)
{
	return 0;
}

void CFriend_TextBox::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CFriend_TextBox::Render()
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

	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_14"), m_strText, XMFLOAT2{ m_fX - (m_fCX / 2.f) + 10.f, m_fY - (m_fCY / 2.f) + 5.f }, XMVECTOR{0.f,0.f, 0.f, 1.f});
	return S_OK;
}

HRESULT CFriend_TextBox::Ready_Component()
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

HRESULT CFriend_TextBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;

	return S_OK;
}

CFriend_TextBox* CFriend_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFriend_TextBox* pInstance = new CFriend_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CFriend_TextBox Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CFriend_TextBox::Clone(void* pArg)
{
	CFriend_TextBox* pInstance = new CFriend_TextBox(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CFriend_TextBox Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFriend_TextBox::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}

