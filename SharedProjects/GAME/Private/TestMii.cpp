#include "TestMii.h"
#include "GameInstance.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_SPHERE.h"

CTestMii::CTestMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CTestMii::CTestMii(const CTestMii& rhs)
	:CActor(rhs)
{
}

HRESULT CTestMii::Initialize_Origin()
{
	return S_OK;
}

HRESULT CTestMii::Initialize_Clone(void* pArg)
{
	m_eInfo.arrEyebrowState[0] = 1;
	m_eInfo.fFat = 1.f;

	nlohmann::json j = {};

	m_pGameInstance->FileManager_Read_File(
		"../../SharedProjects/GAME/GameData/Mii_Data.json",
		j);

	m_eInfo = j.get<MII_INFO>();

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CTestMii::Update_Priority(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_H) & 0x8000)
	{
		MSG_BOX("됐나?");

		/*nlohmann::json j = m_eInfo;
		m_pGameInstance->FileManager_Write_File(
			"../../SharedProjects/GAME/GameData/Mii_Data.json",
			j);*/
	}

	static bool istmp = FALSE;

	if (!istmp)
	{
		istmp = TRUE;
		m_isTmp = TRUE;
	}
	if (false)
	{
	if (m_pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_pTransformCom->Go_Straight(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_pTransformCom->Go_Backward(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->TurnY(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_pTransformCom->TurnY(-fDT);
	}

	}

	///* Change Animation 을 하고 PlayAnimation 권장 */
	//if (m_pGameInstance->Get_DIKeyState(DIK_1))
	//{
	//	m_pSeModel->Change_Animation("CM_Call", 0.2f, TRUE);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_2))
	//{
	//	m_pSeModel->Change_Animation("CM_JumpDelight", 0.2f, TRUE);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_3))
	//{
	//	m_pSeModel->Change_Animation("CM_Walk01", 0.2f, TRUE);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_4))
	//{
	//	m_pSeModel->Change_Animation("CM_Wait03", 0.2f, TRUE);
	//}
	//if (m_pGameInstance->Get_DIKeyState(DIK_5))
	//{
	//	m_pSeModel->Change_Animation("CM_Run00", 0.2f, TRUE);
	//}

	return 0;
}

int32_t CTestMii::Update(_float fDT)
{
	m_pCollider->Update_Collider();

#ifdef ANIM_TEST
	 m_pSeModel->Play_Animation(fDT);
#else
	m_pModelCom->Set_Animation(0, TRUE);
	m_pModelCom->Play_Animation(fDT);

#endif // ANIM_TEST
	return 0;
}

void CTestMii::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CTestMii::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



#ifdef ANIM_TEST
	uint32_t iNumMesh = m_pSeModel->Get_NumMesh();

#else
	uint32_t iNumMesh = m_pModelCom->Get_NumMesh();

#endif // ANIM_TEST

	for (uint32_t i = 0; i < iNumMesh; i++)
	{
#ifdef ANIM_TEST
		if (FAILED(m_pSeModel->Bind_Material(
			m_pShaderCom,
			"g_DiffuseTexture",
			i,
			se_TextureType_DIFFUSE,
			0)))
			return E_FAIL;

		if (FAILED(m_pSeModel->Bind_Bones(m_pShaderCom,
			"g_BoneMatrices",
			i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		m_pSeModel->Render_SeModel(i);
#else
		if (FAILED(m_pModelCom->Bind_Material(
			m_pShaderCom,
			"g_DiffuseTexture",
			i,
			aiTextureType_DIFFUSE,
			0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Bones(m_pShaderCom,
			"g_BoneMatrices",
			i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		m_pModelCom->Render(i);
#endif // ANIM_TEST

	}

#ifdef _DEBUG
	m_pCollider->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTestMii::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_VtxAnimMeshShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;

#ifdef ANIM_TEST
	//if (FAILED(Add_Component(
	//	static_cast<uint32_t>(PLACEID::STATIC),
	//	TEXT("ProtoTag_Component_seBabyJumper"),
	//	TEXT("Com_ModelCom"),
	//	reinterpret_cast<CComponent**>(&m_pSeModel),
	//	nullptr)))
	//	return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_seCamouflageJacket"),
		TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pSeModel),
		nullptr)))
		return E_FAIL;

	//if (FAILED(Add_Component(
	//	static_cast<uint32_t>(PLACEID::CITYHALL),
	//	TEXT("ProtoTag_Component_BasicManBody_SEModel"),
	//	TEXT("Com_ModelCom"),
	//	reinterpret_cast<CComponent**>(&m_pSeModel),
	//	nullptr)))
	//	return E_FAIL;


#else
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACE_ID::STATIC),
		TEXT("ProtoTag_Component_ChineseDress"),
		TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pModelCom),
		nullptr)))
		return E_FAIL;

#endif // ANIM_TEST

	// CBounding_AABB::AABB_DESC BoundDesc = {};
	// BoundDesc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };
	// BoundDesc.vExtents = XMFLOAT3{ 1.f, 1.f, 1.f };


	//CBounding_OBB::OBB_DESC BoundDesc = {};
	//BoundDesc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };
	//BoundDesc.vExtents = XMFLOAT3{ 1.f, 1.f, 1.f };
	//BoundDesc.vAngle = XMFLOAT3{ 0.f, 45.f, 0.f };

	CBounding_Sphere::SPHERE_DESC BoundDesc = {};
	BoundDesc.fRadius = 1.f;
	BoundDesc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };

	CCollider::COLLIDER_DESC ColliderDesc = {};
	ColliderDesc.pBoundDesc = &BoundDesc;
	ColliderDesc.iColliderLayer = COL_OBJECTS;
	ColliderDesc.iCollisionMask = COL_TRIGGER;
	ColliderDesc.pOwnerWorldMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::DEVELOPER), TEXT("ProtoTag_Component_SPHERECollider"), TEXT("Com_Collider"),
		reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CollsionManager_Add_Collider(static_cast<uint32_t>(PLACEID::DEVELOPER), COLLISION::OBJECT, m_pCollider)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestMii::Bind_ShaderResources()
{
	if(FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom,"g_WorldMatrix")))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom,"g_ViewMatrix",TEXT("World_Camera"),D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom,"g_ProjMatrix",TEXT("World_Camera"),D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShaderCom,"g_vCamPosition",TEXT("World_Camera"))))
		return E_FAIL;
	return S_OK;
}

CTestMii* CTestMii::Create(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
{
	CTestMii* pInstance = new CTestMii(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Mii Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CTestMii::Clone(void* pArg)
{
	CTestMii* pInstance = new CTestMii(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Mii Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTestMii::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSeModel);
	Safe_Release(m_pCollider);

	CActor::Free();
}
