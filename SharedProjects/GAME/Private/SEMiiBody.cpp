#include "SEMiiBody.h"
#include "GameInstance.h"
#include "Bounding_SPHERE.h"
#include "GameDirector.h"

#include "Mii.h"
#include "SEMii.h"

#include "Engine_FlagHelper.h"

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
	m_pChangeState = pDesc->pChangeState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CSEMiiBody::Update_Priority(_float fDT)
{
	/* Receive Food가 아니라면*/
	if (!Has_Any_Flag(*m_pParentState, CSEMii::STATE::RECEIVE_FOOD))
	{
		m_isEatingFood =  FALSE ;
		m_isFin_EatingFood =  FALSE ;
	}

	if (m_pAnimationPlayer->Get_isAnimFin())
		cout << "ANIMFIN :: TRUE" << endl;
	else
		cout << "ANIMFIN : FALSE" << endl;

	if (m_isEatingFood)
	{
		if (m_pAnimationPlayer->Get_isAnimFin())
		{
			if (m_strCurAnimTag != TEXT("Chr_Eat") && m_strCurAnimTag != TEXT("Chr_Eat_Hold"))
			{
				m_isFin_EatingFood = TRUE;
				m_isEatingFood = FALSE;
			}
		}
	}

	return 0;
}

int32_t CSEMiiBody::Update(_float fDT)
{
	m_pCollider->Update_Collider();

	if (*m_pChangeState)
	{
		if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::RECEIVE_FOOD))
			m_pAnimationPlayer->Begin_Macro(TEXT("RecieveFood_Best"));
	}


	return 0;
}

void CSEMiiBody::Update_Late(_float fDT)
{



	m_pAnimationPlayer->Update(fDT);
	SetUp_CombinedWorldMatrix(XMLoadFloat4x4(m_pParentMatrix));

	m_strCurAnimTag = m_pAnimationPlayer->Get_CurAnimTag();

	/* 매크로가 플레이중일 때 */
	if (!m_pAnimationPlayer->Get_isEndMacro())
	{
		/* 먹는중 */
		if (m_strCurAnimTag == TEXT("Chr_Eat") || m_strCurAnimTag == TEXT("Chr_Eat_Hold"))
			m_isEatingFood = TRUE;
	}

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
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	/* Shader */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxAnimMesh_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_seChineseDress"), TEXT("Com_ModelCom"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	/* Collider */
	CBounding_Sphere::SPHERE_DESC BoundDesc = {};
	BoundDesc.fRadius = 1.f;
	BoundDesc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };

	CCollider::COLLIDER_DESC ColliderDesc = {};
	ColliderDesc.pBoundDesc = &BoundDesc;
	ColliderDesc.iColliderLayer = COL_TRIGGER;
	ColliderDesc.iCollisionMask = COL_OBJECTS;
	ColliderDesc.pOwnerWorldMatrix = &m_CombinedWorldMatrix;

	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_SphereCollider"), TEXT("Com_Collider"),
		reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CollsionManager_Add_Collider(static_cast<uint32_t>(PLACEID::STATIC), COLLISION::OBJECT, m_pCollider)))
		return E_FAIL;

	/* 애니메이션 플레이어 */
	/* ProtoTag_Component_AnimationPlayer */
	{
		CAnimationPlayer::ANIMATIONPLAYER_DESC Desc = {};
		Desc.m_pBones = m_pSeModel->Get_BonesPtr();
		Desc.strInitAnimTag = TEXT("Chr_Normal_Wait");
		Desc.PreTransformMatrix = m_pSeModel->Get_InitMatrix();
		if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_AnimationPlayer"), TEXT("Com_AnimationPlayer"), reinterpret_cast<CComponent**>(&m_pAnimationPlayer), &Desc)))
			return E_FAIL;
	}
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
	Safe_Release(m_pAnimationPlayer);

	CPartObject::Free();
}
