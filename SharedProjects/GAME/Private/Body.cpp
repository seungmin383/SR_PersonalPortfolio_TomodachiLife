#include "Body.h"
#include "Model.h"
#include "GameInstance.h"

CBody::CBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CBody::CBody(const CBody& rhs)
	:CPartObject(rhs)
{
}

HRESULT CBody::Initialize_Origin()
{
	return S_OK;
}

HRESULT CBody::Initialize_Clone(void* pArg)
{
	BODY_DESC* pBodyDesc = static_cast<BODY_DESC*>(pArg);
	m_pParentState = pBodyDesc->pParentState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CBody::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CBody::Update(_float fDT)
{
	return 0;
}

void CBody::Update_Late(_float fDT)
{
	SetUp_CombinedWorldMatrix(XMLoadFloat4x4(m_pParentMatrix));
	CPartObject::Update_Late(fDT);
}

HRESULT CBody::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint   iNumMeshes = m_pModel->Get_NumMesh();

	for (uint32_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		m_pModel->Render(i);
	}
	return S_OK;
}

const XMFLOAT4X4* CBody::Get_SocketMatrix(const _char* pBoneName)
{
	// 이름 본의 컴바인드 행렬을 들고옴
	return m_pModel->Get_BoneMatrixPtr(pBoneName);
}

HRESULT CBody::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_VtxAnimMesh_Shader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;


	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::APART),
		TEXT("ProtoTag_Component_BasicManBody_Model"),
		TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pModel),
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody::Bind_ShaderResources()
{
	HRESULT hr = {};

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

CBody* CBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody* pInstance = new CBody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CBody Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CBody::Clone(void* pArg)
{
	CBody* pInstance = new CBody(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CBody Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBody::Free()
{
	CPartObject::Free();
	Safe_Release(m_pModel);
	Safe_Release(m_pSeModel);
	Safe_Release(m_pShaderCom);
}
