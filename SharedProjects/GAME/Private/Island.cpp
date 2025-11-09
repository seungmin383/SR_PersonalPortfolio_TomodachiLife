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
	return S_OK;
}

HRESULT CIsland::Initialize_Clone(void* pArg)
{

	if (FAILED(CActor::Initialize_Clone(pArg)))
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
	int a = 0;
	return 0;
}

void CIsland::Update_Late(_float fDT)
{
	// m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::TOP_NONBLEND, this);
	 CActor::Update_Late(fDT);
}

HRESULT CIsland::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t iNumMeshes = m_pModel->Get_NumMesh();
	
	for (uint32_t i = 0; i < iNumMeshes; i++)
	{
		m_pModel->Bind_Material(
			m_pShaderCom,
			"g_DiffuseTexture",
			i,
			aiTextureType_DIFFUSE);
		m_pShaderCom->Begin(0);
		m_pModel->Render(i);
	}

	return S_OK;
}

void CIsland::Calc_Object_Place_Point()
{
	//m_vObjectPlaceVertices
}

HRESULT CIsland::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::STATIC),
		TEXT("ProtoTag_Component_VtxMeshShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::ISLAND),
		TEXT("ProtoTag_Component_Island_Model"),
		TEXT("Com_Model"),
		reinterpret_cast<CComponent**>(&m_pModel),
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
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	CActor::Free();
}
