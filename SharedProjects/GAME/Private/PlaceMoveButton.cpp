#include "PlaceMoveButton.h"
#include "GameInstance.h"

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
	CButton::BUTTON_DESC tDesc{};

	tDesc.fX = 0.f;
	tDesc.fY = 0.f;

	tDesc.fCX = 100.f;
	tDesc.fCY = 100.f;

	tDesc.fRotationPerSecond = XMConvertToRadians(30.f);
	tDesc.fSpeedPerSecond = 1.f;

	if (FAILED(CButton::Initialize_Clone(&tDesc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}



//
// UI가 뷰포트의 중앙을 00으로 잡고
// 마우스는 전체 윈도우의 좌상단을 00으로 잡아서 
// UI 피킹이 안됨
// 
// 마우스가 450 넘어가면 0으로 만들고
// 넘길때 이 좌표를 넘기는 식으로 하는게 나을듯
//
// UI는 뷰포트의 좌상단이 00이 되도록 하고
//




int32_t CPlaceMoveButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);	// 마우스랑 확인

	if(m_isOverlappedMouse)
		cout << "isOverlapped? : TRUE" << endl;
	else 
		cout << "isOverlapped? : FALSE" << endl;

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
	m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::BOTTOM_UI, this);
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
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Vtx_TexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_UIButtonTexture"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBufferCom),
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

	m_pTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_Texture",
		0);

	return hr;
}

void CPlaceMoveButton::On_Click()
{
	MSG_BOX("오 된다");
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
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


