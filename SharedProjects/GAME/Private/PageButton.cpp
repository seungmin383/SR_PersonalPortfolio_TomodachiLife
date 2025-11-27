#include "PageButton.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"

CPageButton::CPageButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CPageButton::CPageButton(const CPageButton& rhs)
	:CButton(rhs)
{
}

HRESULT CPageButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPageButton::Initialize_Clone(void* pArg)
{
	PAGEBUTTON_DESC* pDesc = static_cast<PAGEBUTTON_DESC*>(pArg);
	m_pClickStrategy = pDesc->pStrategy;
	m_iTextureIndex = pDesc->iTextureIndex;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CPageButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CPageButton::Update(_float fDT)
{
	if (m_isClicked)
		On_Click();

	return UPDATE_CLEAR;
}

void CPageButton::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CPageButton::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPageButton::Ready_Component()
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
		static_cast<uint32_t>(PLACEID::CITYHALL),
		TEXT("ProtoTag_Component_PageButtonTexture"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture),
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPageButton::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;


	if (m_isSelected)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex + 1);
	}
	else if (!m_isSelected)
	{
		m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex);
	}

	return S_OK;;
}

int32_t CPageButton::On_Click()
{
	/* 이 버튼이 선택이 안되어 있었는데 클릭 되었다면 -> 아웃라인 생성 및 타겟 포인트로 확대 */
	if (!m_isSelected)
	{
		m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"));
		m_isSelected = TRUE;
		m_pClickStrategy->OnClick();
	}

	/* 이 버튼이 선택되어 있었는데 클릭되었다면 -> 씬 전환*/
	else
	{
		return PLACE_CHANGE;
	}

	return UPDATE_CLEAR;
}

CPageButton* CPageButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPageButton* pInstance = new CPageButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CPageButton Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CPageButton::Clone(void* pArg)
{
	CPageButton* pInstance = new CPageButton(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CPageButton Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPageButton::Free()
{
	Safe_Release(m_pClickStrategy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pIconTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


