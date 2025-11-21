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
	m_pClickStrategy = tCommonButtonDesc->pClickStrategy;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CCommonButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);	// 마우스랑 확인

	// 커서랑 버튼이 겹쳐있고, 좌클릭이 눌렀다 때졌을 때
	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CCommonButton::Update(_float fDT)
{
	On_Click();

	return UPDATE_CLEAR;
}

void CCommonButton::Update_Late(_float fDT)
{
	//m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::BOTTOM_UI, this);
	CUI::Update_Late(fDT);
}

HRESULT CCommonButton::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCommonButton::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::STATIC),
		TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::STATIC),
		TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBufferCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::ISLAND),
		TEXT("ProtoTag_Component_PlaceMoveButton_Texture"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::ISLAND),
		TEXT("ProtoTag_Component_OutLine_Texture"),
		TEXT("Com_OutLineTexture"),
		reinterpret_cast<CComponent**>(&m_pOutlineTextureCom),
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCommonButton::Bind_ShaderResources()
{
	HRESULT hr = {};

	hr = m_pTransformCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_WorldMatrix");

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ViewMatrix",
		TEXT("UI_Camera"),
		D3DTS::VIEW);

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ProjMatrix",
		TEXT("UI_Camera"),
		D3DTS::PROJ);

	return hr;
}

int32_t CCommonButton::On_Click()
{
	m_pClickStrategy->OnClick();
	return UPDATE_CLEAR;
}

CCommonButton* CCommonButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCommonButton* tmp = new CCommonButton(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CCommonButton Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CCommonButton::Clone(void* pArg)
{
	CCommonButton* tmp = new CCommonButton(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CCommonButton Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CCommonButton::Free()
{
	Safe_Release(m_pClickStrategy);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pOutlineTextureCom);
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


