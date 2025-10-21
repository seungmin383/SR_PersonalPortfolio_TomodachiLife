#include "Island.h"
#include "GameInstance.h"

CIsland::CIsland(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CIsland::CIsland(const CIsland& rhs)
	:CActor(rhs)
{
}

HRESULT CIsland::Initialize_Origin()
{
	int a = 0;
	return S_OK;
}

HRESULT CIsland::Initialize_Clone(void* pArg)
{
	ACTOR_DESC tDesc = {};

	tDesc.fRotationPerSecond = XMConvertToRadians(30.f);
	tDesc.fSpeedPerSecond = 10.f;

	if (FAILED(CActor::Initialize_Clone(&tDesc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CIsland::Update_Priority(_float fDT)
{
	int a = 0;
	return 0;
}

int32_t CIsland::Update(_float fDT)
{
	return 0;
}

void CIsland::Update_Late(_float fDT)
{
	m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::TOP_NONBLEND, this);
}

HRESULT CIsland::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//D3D11_RASTERIZER_DESC r = {};
	//r.CullMode = D3D11_CULL_NONE;
	//r.FrontCounterClockwise = FALSE;
	//ID3D11RasterizerState* rs;
	//m_pDevice->CreateRasterizerState(&r, &rs);
	//m_pContext->RSSetState(rs);

	if (FAILED(m_pTerrainBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;


	if (FAILED(m_pTerrainBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIsland::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Vtx_Normal_TexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_IslandTexture"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_TerrainBuffer"),
		TEXT("Com_TerrainBuffer"),
		reinterpret_cast<CComponent**>(&m_pTerrainBufferCom),
		nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CIsland::Bind_ShaderResources()
{
	HRESULT hr = {};

	hr = m_pTransformCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_WorldMatrix");
	
	m_pGameInstance->PipeLine_Bind_PipeLine_Matrix(
		m_pShaderCom,
		"g_ViewMatrix",
		D3DTS::VIEW);

	m_pGameInstance->PipeLine_Bind_PipeLine_Matrix(
		m_pShaderCom,
		"g_ProjMatrix",
		D3DTS::PROJ);

	m_pTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_Texture",
		0);

	return hr;
}

CIsland* CIsland::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIsland* tmp = new CIsland(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Island Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CIsland::Clone(void* pArg)
{
	CIsland* tmp = new CIsland(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Island Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CIsland::Free()
{
	CActor::Free();
}
