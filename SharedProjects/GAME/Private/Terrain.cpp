#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	:CActor(rhs)
{
}

HRESULT CTerrain::Initialize_Origin()
{
	int a = 0;
	return S_OK;
}

HRESULT CTerrain::Initialize_Clone(void* pArg)
{

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CTerrain::Update_Priority(_float fDT)
{
	int a = 0;
	return 0;
}

int32_t CTerrain::Update(_float fDT)
{
	int a = 0;
	return 0;
}

void CTerrain::Update_Late(_float fDT)
{
	// m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::TOP_UI, this);
	m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::TOP_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTerrainBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pTerrainBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Ready_Component()
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
		reinterpret_cast<CComponent**>(&m_pTextureCom[static_cast<uint32_t>(TEXTURE_TYPE::DIFFUSE)]),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_IslandMaskTexture"),
		TEXT("Com_MaskTexture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom[static_cast<uint32_t>(TEXTURE_TYPE::MASK)]),
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

HRESULT CTerrain::Bind_ShaderResources()
{
	HRESULT hr = {};

	hr = m_pTransformCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_WorldMatrix");

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

	m_pGameInstance->CameraManager_Bind_Camera_Pos(
		m_pShaderCom,
		"g_vCamPosition",
		TEXT("World_Camera"));

	m_pTextureCom[static_cast<uint32_t>(TEXTURE_TYPE::DIFFUSE)]->
		Bind_ShaderResources(
		m_pShaderCom,
		"g_DiffuseTexture");

	m_pTextureCom[static_cast<uint32_t>(TEXTURE_TYPE::MASK)]->
		Bind_ShaderResource(
			m_pShaderCom,
			"g_MaskTexture",
			0);

	return hr;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* tmp = new CTerrain(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Island Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CTerrain::Clone(void* pArg)
{
	CTerrain* tmp = new CTerrain(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Island Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CTerrain::Free()
{
	Safe_Release(m_pShaderCom);

	for(auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pTerrainBufferCom);
	CActor::Free();
}
