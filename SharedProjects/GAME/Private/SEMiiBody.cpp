#include "SEMiiBody.h"
#include "GameInstance.h"
#include "Bounding_SPHERE.h"

#include "Mii.h"

CSEMiiBody::CSEMiiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CSEMiiBody::CSEMiiBody(const CSEMiiBody& rhs)
	:CPartObject(rhs)
{
}

HRESULT CSEMiiBody::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSEMiiBody::Initialize_Clone(void* pArg)
{
	SEMIIBODY_DESC* pDesc = static_cast<SEMIIBODY_DESC*>(pArg);
	m_pParentState = pDesc->pParnetState;
	m_pParentClothState = pDesc->pParentClothState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CSEMiiBody::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CSEMiiBody::Update(_float fDT)
{
	m_pCollider->Update_Collider();

	if (static_cast<int32_t>(STATE::IDLE) & *m_pParentState)
		m_pSeModel->Change_Animation("CM_Wait00", 0.2f, TRUE);
	if (static_cast<int32_t>(STATE::WALK) & *m_pParentState)
		m_pSeModel->Change_Animation("CM_Walk01", 0.2f, TRUE);
	if (static_cast<int32_t>(STATE::RUN )& *m_pParentState)
		m_pSeModel->Change_Animation("CM_Run00", 0.2f, TRUE);
	if (static_cast<int32_t>(STATE::Greet )& *m_pParentState)
		m_pSeModel->Change_Animation("CM_Greet00", 0.2f, TRUE);

	if (m_pGameInstance->Get_DIKeyState(DIK_B))
	{
		if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_BabyJumper"), TEXT("Com_ModelCom"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
			return E_FAIL;
	}
	return 0;
}

void CSEMiiBody::Update_Late(_float fDT)
{
	if (m_pCollider->Get_isTriggerEnter())
		MSG_BOX("OnTriggerEnter");
	if (m_pCollider->Get_isTriggerExit())
		MSG_BOX("OnTriggerExit");

	m_pSeModel->Play_Animation(fDT);
	SetUp_CombinedWorldMatrix(XMLoadFloat4x4(m_pParentMatrix));
	CPartObject::Update_Late(fDT);
}

HRESULT CSEMiiBody::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pSeModel->Bind_Bones(m_pShader, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pSeModel->Render_SeModel(i)))
			return E_FAIL;
	}
#ifdef _DEBUG
	m_pCollider->Render();
#endif
	return S_OK;
}

HRESULT CSEMiiBody::Ready_Component()
{
	/* Shader */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxAnimMeshShader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_seChineseDress"), TEXT("Com_ModelCom"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	CBounding_Sphere::SPHERE_DESC BoundDesc = {};
	BoundDesc.fRadius = 1.f;
	BoundDesc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };

	CCollider::COLLIDER_DESC ColliderDesc = {};
	ColliderDesc.pBoundDesc = &BoundDesc;
	ColliderDesc.iColliderLayer = COL_TRIGGER;
	ColliderDesc.iCollisionMask = COL_OBJECTS;
	ColliderDesc.pOwnerWorldMatrix = &m_CombinedWorldMatrix;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_SphereCollider"), TEXT("Com_Collider"),
		reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CollsionManager_Add_Collider(static_cast<uint32_t>(PLACEID::STATIC), COLLISION::TRIGGER, m_pCollider)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSEMiiBody::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShader, "g_vCamPosition", TEXT("World_Camera"))))
		return E_FAIL;
	return S_OK;
}

const XMFLOAT4X4* CSEMiiBody::Get_SocketMatrix(const _char* pBoneName)
{
	return m_pSeModel->Get_BoneCombinedTransformMatrixPtr(pBoneName);
}

CSEMiiBody* CSEMiiBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMiiBody* pInstance = new CSEMiiBody(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMiiBody Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMiiBody::Clone(void* pArg)
{
	CSEMiiBody* pInstance = new CSEMiiBody(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMiiBody Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEMiiBody::Free()
{
	Safe_Release(m_pSeModel);
	Safe_Release(m_pShader);
	Safe_Release(m_pCollider);

	CPartObject::Free();
}
