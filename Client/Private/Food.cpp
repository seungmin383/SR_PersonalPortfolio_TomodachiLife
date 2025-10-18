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
	
	tDesc.fX = 0.f;
	tDesc.fY = 0.f;

	tDesc.fCX = 500.f;
	tDesc.fCY = 500.f;

	tDesc.fRotationPerSecond = XMConvertToRadians(30.f);
	tDesc.fSpeedPerSecond = 10.f;

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
	m_pTransformCom->Go_Left(fDT);

	return 0;
}

void CFood::Update_Late(_float fDT)
{
	m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::NONBLEND, this);

}

HRESULT CFood::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//D3D11_RASTERIZER_DESC r = {};
	//r.CullMode = D3D11_CULL_NONE;
	//r.FrontCounterClockwise = FALSE;
	//ID3D11RasterizerState* rs;
	//m_pDevice->CreateRasterizerState(&r, &rs);
	//m_pContext->RSSetState(rs);

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
		static_cast<uint32_t>(PLACE_ID::STATIC),
		TEXT("ProtoTag_Component_Shader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Texture"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::STATIC),
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

	hr = Bind_OrthoMatrices(
		m_pShaderCom,
		"g_ViewMatrix",
		"g_ProjMatrix");

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
