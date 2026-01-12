#include "SEMiiBody.h"
#include "GameInstance.h"
#include "Bounding_SPHERE.h"
#include "GameDirector.h"

#include "Mii.h"
#include "ApartManager.h"
#include "SEMii.h"

#include "Engine_FlagHelper.h"
#include "NarrationManager.h"

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
	m_pParentState		= pDesc->pParnetState;
	m_pParentClothState = pDesc->pParentClothState;
	m_pChangeState		= pDesc->pChangeState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Normal_Wait"), TRUE, 1.f);
	return S_OK;
}

int32_t CSEMiiBody::Update_Priority(_float fDT)
{
	m_isPlaying_ReceiveFood_Event = FALSE;
	m_isPlaying_BeFriend_Event = FALSE;

	if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::RECEIVE_FOOD))
	{
		m_isPlaying_ReceiveFood_Event = TRUE;
	}
	if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::BE_FRIEND))
	{
		m_isPlaying_BeFriend_Event = TRUE;
	}

	return 0;
}

int32_t CSEMiiBody::Update(_float fDT)
{
	m_pCollider->Update_Collider();

	/* Direction이 바꼈을 때 */
	if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::CHANGE_STATE))
	{
		m_isStart_ReceiveFood_Event = FALSE;

		if (m_isPlaying_ReceiveFood_Event)
		{
			m_isStart_ReceiveFood_Event = TRUE;

			if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::FOOD_BEST))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(TEXT("RecieveFood_Best"));
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::FOOD_GOOD))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(TEXT("RecieveFood_Good"));
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::FOOD_NOTBAD))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(TEXT("RecieveFood_NotBad"));
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::FOOD_SOSO))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(TEXT("RecieveFood_SoSo"));
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::FOOD_BAD))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(TEXT("RecieveFood_Bad"));
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::FOOD_WORST))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(TEXT("RecieveFood_Worst"));
		}
		else if (m_isPlaying_BeFriend_Event)
		{
			if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::WALKING))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Normal_Walk"), TRUE, 1.f);
			if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::BANTER))
			{
				_wstring str = L"Banter_" + to_wstring(m_pGameInstance->Real_Random(1, 5));
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Macro(str);
			}
			if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::HAPPY))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Happy_Wait"), TRUE, 2.f);

		}
		else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::LOVE))
		{
			if(Has_Any_Flag(*m_pParentState, CSEMii::STATE::PROPOSE))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Romantic_Hold"), FALSE	, 1.f);
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::IDLE_HAPPY))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Happy_Wait"), TRUE, 1.f);
			else if(Has_Any_Flag(*m_pParentState, CSEMii::STATE::SAD))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Sad_Wait"), TRUE, 1.f);
			else if(Has_Any_Flag(*m_pParentState, CSEMii::STATE::HAPPY))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Jump_Delight"), FALSE, 1.f);
			else if(Has_Any_Flag(*m_pParentState, CSEMii::STATE::WALKING))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Normal_Walk"), TRUE, 1.f);
			else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::CALM))
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Normal_Wait"), TRUE, 1.f);

			else
				m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_FallInLove"), TRUE, 1.f);

		}
		else if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::THANK))
		{
			m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Bow"), FALSE, 1.f);
		}
		else
			m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Normal_Wait"), TRUE, 1.f);
	}

	return 0;
}

void CSEMiiBody::Update_Late(_float fDT)
{
	/* 애니메이션 플레이 -> State 계산까지 */
	Calc_BodyState(fDT);

	SetUp_CombinedWorldMatrix(XMLoadFloat4x4(m_pParentMatrix));
	CPartObject::Update_Late(fDT);
}

HRESULT CSEMiiBody::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel[*m_pParentClothState]->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel[*m_pParentClothState]->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pSeModel[*m_pParentClothState]->Bind_Bones(m_pShader, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(VAMS_BODY)))
			return E_FAIL;

		if (FAILED(m_pSeModel[*m_pParentClothState]->Render_SeModel(i)))
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
	m_pSeModel.resize(10);

	_wstring strClothModelTagArr[] = {
	TEXT("BabyJumper"),
	TEXT("CamouflageJacket"),
	TEXT("ChineseDress"),
	TEXT("FauxFarCoat"),
	TEXT("NintendoUniform"),
	TEXT("SilkBlouse"),
	TEXT("SoccerUniform"),
	TEXT("Toga"),
	TEXT("WaitressUniform"),
	};
	

	for (uint32_t i = 0; i < 9; ++i)
	{
		if (FAILED(Add_Component(iStaticID, (L"ProtoTag_Component_se" + strClothModelTagArr[i]), TEXT("Com_ModelCom") + to_wstring(i), reinterpret_cast<CComponent**>(&m_pSeModel[i]), nullptr)))
			return E_FAIL;

	}

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
		Desc.strInitAnimTag		= TEXT("Chr_Normal_Wait");

		m_pAnimationPlayer.resize(10);
		for(uint32_t i = 0; i < 9; ++i)
		{
			Desc.m_pBones			= m_pSeModel[i]->Get_BonesPtr();
			Desc.PreTransformMatrix = m_pSeModel[i]->Get_InitMatrix();
			if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_AnimationPlayer"), TEXT("Com_AnimationPlayer") + to_wstring(i), reinterpret_cast<CComponent**>(&m_pAnimationPlayer[i]), &Desc)))
				return E_FAIL;
		}
	}


	{
		if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_Cloth_MaskTexture"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pMaskTexture), nullptr)))
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
	if (FAILED(m_pMaskTexture->Bind_ShaderResource(m_pShader, "g_MaskTexture", 0)))
		return E_FAIL;	
	return S_OK;
}

void CSEMiiBody::Calc_BodyState(_float fDT)
{
	m_pAnimationPlayer[*m_pParentClothState]->Update(fDT);

	m_strCurAnimTag = m_pAnimationPlayer[*m_pParentClothState]->Get_CurAnimTag();

	/* Receive Food 이벤트가 플레이 중 이라면 */
	if (m_isPlaying_ReceiveFood_Event)
	{
		/* 관련 플래그 초기화 */
		m_isEatingFood		= FALSE;
		m_isReceiveFood		= FALSE;
		m_isFin_EatingFood	= FALSE;
		m_isReactingTaste	= FALSE;

		/* 음식을 받았는지 */
		if (m_strCurAnimTag.find(TEXT("Chr_ItemReceive_Hold")) != _wstring::npos)
		{
			m_isReceiveFood = TRUE;
		}

		/* 음식을 먹고 있는지 */
		else if (m_strCurAnimTag.find(TEXT("Chr_Eat")) != _wstring::npos)
		{
			m_isEatingFood = TRUE;
		}

		/* 음식을 다 먹었는지 */
		else if (m_strCurAnimTag.find(TEXT("Chr_SwipeMouth")) != _wstring::npos)
		{
			m_isFin_EatingFood = TRUE;
		}

		/* 맛평가를 하고 있는지 */
		else if (m_strCurAnimTag.find(TEXT("Chr_Food")) != _wstring::npos)
		{
			m_isReactingTaste = TRUE;
		}

	}
	else if (m_isPlaying_BeFriend_Event)
	{
		m_isFin_Banter = FALSE;


		/* 대화가 끝났는지 */
		if (m_strCurAnimTag.find(TEXT("Chr_Bow_Hold")) != _wstring::npos)
		{
			m_isFin_Banter = TRUE;
		}
	}
	else if (m_strCurAnimTag == (TEXT("Chr_Bow")))
	{
		m_isThankTalking = TRUE;
	}

	if (m_isThankTalking)
	{
		if (m_pAnimationPlayer[*m_pParentClothState]->Get_isAnimFin())
		{
			m_pAnimationPlayer[*m_pParentClothState]->Begin_Animation(TEXT("Chr_Normal_Wait"), TRUE, 1.f);
			
			CApartManager::GetInstance()->Set_isThankTalking(FALSE);
			m_isThankTalking = FALSE;
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
		}
	}
}

const XMFLOAT4X4* CSEMiiBody::Get_SocketMatrix(const _char* pBoneName)
{
	return m_pSeModel[*m_pParentClothState]->Get_BoneCombinedTransformMatrixPtr(pBoneName);
}

vector<const XMFLOAT4X4*>* CSEMiiBody::Get_SocketMatrices(const _char* pBoneName)
{
	for (uint32_t i = 0; i < 9; ++i)
	{
		m_SoketMatrices.push_back(m_pSeModel[i]->Get_BoneCombinedTransformMatrixPtr(pBoneName));
	}
	return &m_SoketMatrices;
}

_bool CSEMiiBody::Get_isFin_Macro() const
{
	return m_pAnimationPlayer[*m_pParentClothState]->Get_isEndMacro();
}

_bool CSEMiiBody::Get_isFin_Anim() const
{
	return m_pAnimationPlayer[*m_pParentClothState]->Get_isAnimFin();
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
	for(auto& p : m_pSeModel)
		Safe_Release(p);
	m_pSeModel.clear();

	Safe_Release(m_pShader);
	Safe_Release(m_pMaskTexture);
	Safe_Release(m_pCollider);

	for(auto& p : m_pAnimationPlayer)
		Safe_Release(p);
	m_pAnimationPlayer.clear();



	CPartObject::Free();
}
