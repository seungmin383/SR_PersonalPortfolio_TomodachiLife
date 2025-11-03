#include "PlaceMoveButton.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CPlaceMoveButton::CPlaceMoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CPlaceMoveButton::CPlaceMoveButton(const CPlaceMoveButton& rhs)
	:CButton(rhs)
{
}

HRESULT CPlaceMoveButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPlaceMoveButton::Initialize_Clone(void* pArg)
{
	PLACEMOVEBUTTON_DESC* tPlaceMoveButtonDesc = static_cast<PLACEMOVEBUTTON_DESC*>(pArg);
	
	m_eNextPlaceID = tPlaceMoveButtonDesc->eNextPlace;
	m_iTextureNum = tPlaceMoveButtonDesc->iTextureNum;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CPlaceMoveButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);	// 마우스랑 확인

	// 커서랑 버튼이 겹쳐있고, 좌클릭이 눌렀다 때졌을 때
	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}
	
	return 0;
}

int32_t CPlaceMoveButton::Update(_float fDT)
{
	if (m_isClicked)
		On_Click();

	return 0;
}

void CPlaceMoveButton::Update_Late(_float fDT)
{
	//m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::BOTTOM_UI, this);
	CUI::Update_Late(fDT);
}

HRESULT CPlaceMoveButton::Render()
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

HRESULT CPlaceMoveButton::Ready_Component()
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

HRESULT CPlaceMoveButton::Bind_ShaderResources()
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

	// 버튼 텍스쳐
	m_pTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_Texture",
		0);

	// 선택 됐는지 안됐는지
	m_pOutlineTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_OutlineTexture",
		0);

	return hr;
}

void CPlaceMoveButton::On_Click()
{
	m_pGameInstance->PlaceManager_Change_Place(
		static_cast<uint32_t>(m_eNextPlaceID),
		CPlace_Loading::Create(
			m_pDevice,
			m_pContext,
			m_eNextPlaceID));

}

CPlaceMoveButton* CPlaceMoveButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlaceMoveButton* tmp = new CPlaceMoveButton(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CPlaceMoveButton::Clone(void* pArg)
{
	CPlaceMoveButton* tmp = new CPlaceMoveButton(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CPlaceMoveButton::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pOutlineTextureCom);
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


