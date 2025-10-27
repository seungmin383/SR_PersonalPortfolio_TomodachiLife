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
	Ready_Component();
	CActor::Initialize_Clone(pArg);

	return S_OK;
}

int32_t CMii::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CMii::Update(_float fDT)
{
	return 0;
}

void CMii::Update_Late(_float fDT)
{
}

HRESULT CMii::Render()
{
	return E_NOTIMPL;
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
		TEXT("Com_TerrainBuffer"),
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
	CActor::Free();
}
