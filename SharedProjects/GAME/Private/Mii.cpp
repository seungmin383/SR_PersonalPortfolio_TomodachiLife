#include "Mii.h"
#include "GameInstance.h"

CMii::CMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CMii::CMii(const CMii& rhs)
	:CActor(rhs)
{
}

HRESULT CMii::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMii::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CMii::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CMii::Update(_float fDT)
{
	int a = 0;
	return 0;
}

void CMii::Update_Late(_float fDT)
{
	m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::TOP_NONBLEND, this);
}

HRESULT CMii::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	uint32_t iNumMesh = m_pModelCom->Get_NumMesh();

	for (size_t i = 0; i < iNumMesh; i++)
	{
		m_pModelCom->Bind_Material(
			m_pShaderCom,
			"g_DiffuseTexture",
			i,
			aiTextureType_DIFFUSE,
			0);

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CMii::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Vtx_Mesh_TexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_TestModel"),
		TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pModelCom),
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMii::Bind_ShaderResources()
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

CMii* CMii::Create(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
{
	CMii* tmp = new CMii(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Mii Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CMii::Clone(void* pArg)
{
	CMii* tmp = new CMii(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Mii Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CMii::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	CActor::Free();
}
