#include "Food.h"
#include "GameInstance.h"

CFood::CFood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CFood::CFood(const CFood& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CFood::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFood::Initialize_Clone(void* pArg)
{
	CUI::UI_DESC tDesc{};
	
	uint32_t iNum = {2};
	D3D11_VIEWPORT Viewport_Desc[2] = {};

	m_pContext->RSGetViewports(&iNum, Viewport_Desc);

	tDesc.fX = 0;
	tDesc.fY = 0;

	tDesc.fCX = 10.f;
	tDesc.fCY = 10.f;

	tDesc.fRotationPerSecond = XMConvertToRadians(30.f);
	tDesc.fSpeedPerSecond = 1.f;

	if (FAILED(CUI::Initialize_Clone(&tDesc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CFood::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CFood::Update(_float fDT)
{
	m_pTransformCom->Go_Down(fDT);

	return 0;
}

void CFood::Update_Late(_float fDT)
{
	m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::TOP_NONBLEND, this);
	//m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::BOTTOM_UI, this);
}

HRESULT CFood::Render()
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

HRESULT CFood::Ready_Component()
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
		TEXT("ProtoTag_Component_FoodTexture"),
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

HRESULT CFood::Bind_ShaderResources()
{
	HRESULT hr = {};

	hr = m_pTransformCom->Bind_ShaderResource(
		m_pShaderCom, 
		"g_WorldMatrix");

	//hr = Bind_OrthoMatrices(
	//	m_pShaderCom,
	//	"g_ViewMatrix",
	//	"g_ProjMatrix");

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ViewMatrix",
		TEXT("World_Camera"),
		D3DTS::VIEW);

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ProjMatrix",
		TEXT("World_Camera"),
		D3DTS::PROJ);

	m_pTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_Texture",
		0);

	return hr;
}

CFood* CFood::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFood* tmp = new CFood(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CFood::Clone(void* pArg)
{
	CFood* pFood = new CFood(*this);

	if (FAILED(pFood->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(pFood);
		return nullptr;
	}

	return pFood;
}

void CFood::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
