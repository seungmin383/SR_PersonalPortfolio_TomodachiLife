#include "CommonButton.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"

CCommonButton::CCommonButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CCommonButton::CCommonButton(const CCommonButton& rhs)
	:CButton(rhs)
{
}

HRESULT CCommonButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CCommonButton::Initialize_Clone(void* pArg)
{
	COMMONBUTTON_DESC* tCommonButtonDesc = static_cast<COMMONBUTTON_DESC*>(pArg);
	m_eTexturePlaceID = tCommonButtonDesc->eTexturePlaceID;

	m_pClickStrategy = tCommonButtonDesc->pStrategy;
	
	m_strTextureTag = tCommonButtonDesc->strTextureTag;
	m_iTextureIndex = tCommonButtonDesc->iTextureIndex;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CCommonButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CCommonButton::Update(_float fDT)
{
	if(m_isClicked)
		On_Click();

	return UPDATE_CLEAR;
}

void CCommonButton::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CCommonButton::Render()
{
	__super::Render();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		if (FAILED(m_pShader->Begin(5)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShader->Begin(2)))
			return E_FAIL;
	}

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCommonButton::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShader),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBuffer),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(m_eTexturePlaceID),
		m_strTextureTag,
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture),
		nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CCommonButton::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex + 1);
	}
	else if (m_isClicked)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex + 1);
	}
	else
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex);
	}

	return S_OK;;
}

int32_t CCommonButton::On_Click()
{
	m_pClickStrategy->OnClick();
	m_isClicked = FALSE;

	return UPDATE_CLEAR;
}

CCommonButton* CCommonButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCommonButton* pInstance = new CCommonButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CCommonButton Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CCommonButton::Clone(void* pArg)
{
	CCommonButton* pInstance = new CCommonButton(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CCommonButton Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCommonButton::Free()
{
	Safe_Release(m_pClickStrategy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


