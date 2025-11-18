#include "Weapon.h"
#include "GameInstance.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon& rhs)
	:CPartObject(rhs)
{
}

HRESULT CWeapon::Initialize_Origin()
{
	return S_OK;
}

HRESULT CWeapon::Initialize_Clone(void* pArg)
{
	WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

int32_t CWeapon::Update_Priority(_float fDT)
{
	CPartObject::Update_Priority(fDT);
	return 0;
}

int32_t CWeapon::Update(_float fDT)
{
	CPartObject::Update(fDT);

	return 0;
}

void CWeapon::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);

	XMMATRIX ParentMatrix =  XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);

	SetUp_CombinedWorldMatrix(ParentMatrix);
}

HRESULT CWeapon::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint   iNumMeshes = m_pModel->Get_NumMesh();

	for (uint32_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		/*m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0);*/

		if (FAILED(m_pModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		m_pModel->Render(i);
	}
	return S_OK;
}

HRESULT CWeapon::Ready_Components()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::STATIC),
		TEXT("ProtoTag_Component_VtxAnimMeshShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::APART),
		TEXT("ProtoTag_Component_GirlHair1_Model"),
		TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pModel),
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::Bind_ShaderResources()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix);

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

	return S_OK;
}


CWeapon* CWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon* pInstance = new CWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CWeapon Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CWeapon::Clone(void* pArg)
{
	CWeapon* pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CWeapon Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
void CWeapon::Free()
{
	CPartObject::Free();
	Safe_Release(m_pModel);
	Safe_Release(m_pSeModel);
	Safe_Release(m_pShaderCom);
}

