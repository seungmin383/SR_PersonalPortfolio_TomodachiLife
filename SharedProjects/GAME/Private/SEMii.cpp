#include "SEMii.h"
#include "SEMiiBody.h"
#include "SEMiiHead.h"
#include "SEMiiHair.h"
#include "SEMiiEyes.h"
#include "SEMiiEyebrow.h"
#include "SEMiiNose.h"
#include "SEMiiMouth.h"

#include "GameInstance.h"
#include "MiiManager.h"
#include "ItemManager.h"

#include "LevelUp_Item.h"
#include "FoodItem_Texture.h"
#include "Food_Item.h"
#include "Engine_FlagHelper.h"
#include "NarrationManager.h"

CSEMii::CSEMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{
}

CSEMii::CSEMii(const CSEMii& rhs)
	:CContainerObject(rhs)
{
}

HRESULT CSEMii::Initialize_Origin()
{

	return S_OK;
}

HRESULT CSEMii::Initialize_Clone(void* pArg)
{
	MII_DESC* pDesc = static_cast<MII_DESC*>(pArg);
	m_strNickName = pDesc->strNickName;
	m_iIndex = pDesc->iIndex;

	if (FAILED(CContainerObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_FoodFlavorFlags.reserve(9);
	for (uint32_t i = 0; i < 9; ++i)
	{
		m_FoodFlavorFlags.push_back(static_cast<int32_t>(CFoodItem_Texture::FOODFLAVOR::SALTY) << i);
	}

	return S_OK;
}

int32_t CSEMii::Update_Priority(_float fDT)
{
	Observe();
	Thinking();

	return CContainerObject::Update_Priority(fDT);
}

int32_t CSEMii::Update(_float fDT)
{
	Acting();

	return CContainerObject::Update(fDT);
}

void CSEMii::Update_Late(_float fDT)
{
	CContainerObject::Update_Late(fDT);
}

HRESULT CSEMii::Render()
{
	return S_OK;
}

void CSEMii::Observe()
{
	/* 전역 스테이트 -> 일종의 타임라인 (어떤 이벤트가 실행중인지 )*/
	m_eDirection = CGameDirector::GetInstance()->Get_Direction();

	if (m_eDirection != m_ePreDirection)
		m_isChangeState = TRUE;
	else
		m_isChangeState = FALSE;

	m_ePreDirection = m_eDirection;

	m_isFin_Macro = m_pBody->Get_isFin_Macro();

	if (m_eDirection == CGameDirector::GAME_DIRECTION::GIVE_FOOD)
	{
		m_isStart_ReceiveFood_Event = m_pBody->Get_isStart_ReceiveFood_Event();

		m_isReceivedFood = m_pBody->Get_isReceivedFood();
		m_isEatingFood = m_pBody->Get_isEatingFood();
		m_isFin_EatingFood = m_pBody->Get_isFin_EatingFood();
		m_isReactingTaste = m_pBody->Get_isReactingTaste();
	}
}

void CSEMii::Thinking()
{
	if (m_isChangeState)
	{
		m_isLevelUp = FALSE;
		m_isFullnessUp = FALSE;
	}

	Clear_Flag(m_iState);

	/* 전역 스테이트로 1차 내부 스테이트를 설정 */
	switch (m_eDirection)
	{
	case Game::CGameDirector::GAME_DIRECTION::NO_EVENT:
		Set_Flag(m_iState, STATE::IDLE);
		break;
	case Game::CGameDirector::GAME_DIRECTION::GIVE_ITEM:
		break;
	case Game::CGameDirector::GAME_DIRECTION::GIVE_FOOD:
		Set_Flag(m_iState, STATE::RECEIVE_FOOD);
		break;

	default:
		break;
	}

	/* 내 정보를 이용해서 2차 내부 스테이트를 추가 */
	if (m_eDirection == CGameDirector::GAME_DIRECTION::GIVE_FOOD)
		About_ReceiveFood();

	/* 파트 오브젝트의 값을 이용해서 3차 내부 스테이트를 추가 */

}

void CSEMii::About_ReceiveFood()
{
	_float fFlavor = 0.f;
	uint32_t iCnt = 0;

	/* 음식이 가진 맛 플래그를 받아옴 */
	const int32_t iFlav = CItemManager::GetInstance()->Get_CurFoodInfo().iFlavor;
	const auto& Profile = CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile;

	for (uint32_t i = 0; i < 9; ++i)
	{
		if (Has_Any_Flag(iFlav, m_FoodFlavorFlags[i]))
		{
			fFlavor += Profile.fFlavor[i];
			++iCnt;

			if (iCnt == 2)
				break;
		}
	}

	if (fFlavor <= 4.f && fFlavor > 3.5f)
		Add_Flag(m_iState, STATE::FOOD_BEST);
	else if (fFlavor <= 3.5f && fFlavor > 2.8f)
		Add_Flag(m_iState, STATE::FOOD_GOOD);
	else if (fFlavor <= 2.8f && fFlavor > 2.1f)
		Add_Flag(m_iState, STATE::FOOD_NOTBAD);
	else if (fFlavor <= 2.1f && fFlavor > 1.2f)
		Add_Flag(m_iState, STATE::FOOD_SOSO);
	else if (fFlavor <= 1.2f && fFlavor > 0.5f)
		Add_Flag(m_iState, STATE::FOOD_BAD);
	else if (fFlavor <= 0.5f && fFlavor >= 0.f)
		Add_Flag(m_iState, STATE::FOOD_WORST);

	/* 만약 이벤트 플레이 중 매크로가 끝났다면 */
	if (m_isStart_ReceiveFood_Event && m_isFin_Macro)
	{
		/* Idle 상태로 바꾸고 */
		CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::NO_EVENT);
		CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);

		Add_Flag(m_iState, STATE::FIN_FOODEVENT);
	}

	if (m_isReceivedFood)
		Add_Flag(m_iState, STATE::RECEIVED_FOOD);

	if (m_isFin_EatingFood || m_isReactingTaste)
		Add_Flag(m_iState, STATE::ATE_FOOD);

	if (m_isEatingFood)
		Add_Flag(m_iState, STATE::EATING_FOOD);

}

void CSEMii::Acting()
{
	if (m_eDirection == CGameDirector::GAME_DIRECTION::GIVE_FOOD)
		For_ReceivedFood();

}

void CSEMii::For_ReceivedFood()
{
	if (m_isReceivedFood)
	{
		CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
		CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f,1.f,1.f });
		CNarrationManager::GetInstance()->Set_NarrationText(TEXT("잘 먹겠습니다!"));
	}

	if (m_isEatingFood)
		CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);


	if (!m_isLevelUp && m_isReactingTaste)
	{
		m_isLevelUp = TRUE;

		if (Has_Any_Flag(m_iState, STATE::FOOD_BEST))
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += 0.7f;
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_GOOD))
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += 0.5f;
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_NOTBAD))
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += 0.3f;
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_SOSO))
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += 0.f;
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_BAD))
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += -0.1f;
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_WORST))
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += -0.2f;
		}
	}

	if (!m_isFullnessUp && m_isFin_EatingFood)
	{
		m_isFullnessUp = TRUE;
		CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iFullness += CItemManager::GetInstance()->Get_CurFoodInfo().iSatiety;
	}

	if (m_isReactingTaste)
	{
		CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);

		if (Has_Any_Flag(m_iState, STATE::FOOD_BEST))
		{
			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("좋아하는 음식이었나 봐요!"));
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_GOOD))
		{
			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("정말 맛 있었나봐요!"));
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_NOTBAD))
		{
			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("그럭저럭 맛 있었나봐요!"));
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_SOSO))
		{
			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("별로였나 봐요."));
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_BAD))
		{
			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("맛 없었나봐요.."));
		}
		else if (Has_Any_Flag(m_iState, STATE::FOOD_WORST))
		{
			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("정말 맛 없었나봐요.."));
		}
	}

	if (m_isStart_ReceiveFood_Event && m_isFin_Macro)
		CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
}


HRESULT CSEMii::Ready_Components()
{
	return S_OK;
}

HRESULT CSEMii::Ready_PartObjects()
{
	m_MiiInfo = CMiiManager::GetInstance()->Get_MiiInfo(m_iIndex);

	/* Part_Body */
	{
		CSEMiiBody::SEMIIBODY_DESC MiiBodyDesc = {};
		MiiBodyDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		MiiBodyDesc.fCX = 1.f;
		MiiBodyDesc.fCY = 1.f;
		MiiBodyDesc.fCZ = 1.f;
		MiiBodyDesc.pParnetState = &m_iState;
		MiiBodyDesc.eRenderGroup = m_eRenderGroup;
		MiiBodyDesc.pParentClothState = &m_iClothState;
		MiiBodyDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		MiiBodyDesc.pChangeState = &m_isChangeState;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiBody"), TEXT("Part_Body"), &MiiBodyDesc)))
			return E_FAIL;

		m_pBody = dynamic_cast<CSEMiiBody*>(Find_PartObject(TEXT("Part_Body")));
		if (nullptr == m_pBody)
			return E_FAIL;

	}

	/* Part_Head */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiHead::SEMIIHEAD_DESC seHeadDesc = {};
		seHeadDesc.vPos = XMFLOAT4{ -0.15f, 0.f, 0.f, 1.f };
		seHeadDesc.fCX = 0.09f;
		seHeadDesc.fCY = 0.09f;
		seHeadDesc.fCZ = 0.09f;
		seHeadDesc.fRotationPerSecond = XMConvertToRadians(90.f);
		seHeadDesc.fSpeedPerSecond = 10.f;
		seHeadDesc.eRenderGroup = m_eRenderGroup;
		seHeadDesc.pSocketMatrix = BodySockeMatrix;
		seHeadDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seHeadDesc.pHeadInfo = &m_MiiInfo.HeadInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiHead"), TEXT("Part_Head"), &seHeadDesc)))
			return E_FAIL;
	}

	/* Part_Hair */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiHair::SEMIIHAIR_DESC seHairDesc = {};
		seHairDesc.vPos = XMFLOAT4{ -0.15f, 0.f, 0.f, 1.f };
		seHairDesc.fCX = 0.1f;
		seHairDesc.fCY = 0.1f;
		seHairDesc.fCZ = 0.1f;
		seHairDesc.fRotationPerSecond = XMConvertToRadians(90.f);
		seHairDesc.fSpeedPerSecond = 3.f;
		seHairDesc.eRenderGroup = m_eRenderGroup;
		seHairDesc.pSocketMatrix = BodySockeMatrix;
		seHairDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seHairDesc.pHairInfo = &m_MiiInfo.HairInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiHair"), TEXT("Part_Hair"), &seHairDesc)))
			return E_FAIL;
	}

	/* 플레이어가 들 레벨업 아이템 */
	// ProtoTag_Actor_LevelUp_Item
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("R_handItem");

		CLevelUp_Item::LEVELUPITEM_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.1f, 0.5f, 1.f };
		Desc.fCX = 0.3f;
		Desc.fCY = 0.3f;
		Desc.fCZ = 0.3f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.eRenderGroup = m_eRenderGroup;
		Desc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		Desc.pSocketMatrix = BodySockeMatrix;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_LevelUp_Item"), TEXT("Part_RItem"), &Desc)))
			return E_FAIL;
	}

	/* 플레이어가 들 음식 아이템 */
	// ProtoTag_Actor_Food_Item
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("R_handItem");

		CFood_Item::FOODITEM_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.1f, 0.5f, 1.f };
		Desc.fCX = 0.3f;
		Desc.fCY = 0.3f;
		Desc.fCZ = 0.3f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.eRenderGroup = m_eRenderGroup;
		Desc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		Desc.pSocketMatrix = BodySockeMatrix;
		Desc.pParentState = &m_iState;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_Food_Item"), TEXT("Part_RFood"), &Desc)))
			return E_FAIL;
	}

	/* Part_RightEyes */
	/* Part_LeftEyes */
	/* 0 ~ 59 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiEyes::SEMIIEYES_DESC seEyesDesc = {};
		seEyesDesc.vPos = XMFLOAT4{ 0.18f, -0.12f, -0.23f, 1.f };
		seEyesDesc.fCX = 0.22f;
		seEyesDesc.fCY = 0.22f;
		seEyesDesc.fCZ = 0.22f;
		seEyesDesc.isRight = TRUE;
		seEyesDesc.fBlinkTime = 5.f;
		seEyesDesc.fBlinkDuration = 0.3f;
		seEyesDesc.fSpeedPerSecond = 10.f;
		seEyesDesc.pParentState = &m_iState;
		seEyesDesc.pSocketMatrix = BodySockeMatrix;
		seEyesDesc.eRenderGroup = m_eRenderGroup;
		seEyesDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seEyesDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seEyesDesc.pEyesInfo = &m_MiiInfo.EyesInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyes"), TEXT("Part_Eyes"), &seEyesDesc)))
			return E_FAIL;

		// Left Eyes: y만 변경
		seEyesDesc.vPos.y = 0.12f;
		seEyesDesc.isRight = FALSE;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyes"), TEXT("Part_LeftEyes"), &seEyesDesc)))
			return E_FAIL;
	}

	/* Part_RightEyebrow */
	/* Part_LeftEyebrow*/
	/* 78 ~ 100 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiEyebrow::SEMIIEYEBROW_DESC seEyebrowDesc = {};
		seEyebrowDesc.vPos = XMFLOAT4{ 0.25f, -0.1f, -0.23f, 1.f };
		seEyebrowDesc.fCX = 0.27f;
		seEyebrowDesc.fCY = 0.22f;
		seEyebrowDesc.fCZ = 0.22f;
		seEyebrowDesc.isRight = TRUE;
		seEyebrowDesc.fSpeedPerSecond = 10.f;
		seEyebrowDesc.pParentState = &m_iState;
		seEyebrowDesc.pSocketMatrix = BodySockeMatrix;
		seEyebrowDesc.eRenderGroup = m_eRenderGroup;
		seEyebrowDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seEyebrowDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seEyebrowDesc.pEyebrowInfo = &m_MiiInfo.EyebrowInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyebrow"), TEXT("Part_RightEyebrow"), &seEyebrowDesc)))
			return E_FAIL;

		// Left Eyebrow: y만 변경
		seEyebrowDesc.vPos.y = 0.1f;
		seEyebrowDesc.isRight = FALSE;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyebrow"), TEXT("Part_LeftEyebrow"), &seEyebrowDesc)))
			return E_FAIL;
	}

	/* Part_Nose */
	/* 60 ~ 77 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiNose::SEMIINOSE_DESC seNoseDesc = {};
		seNoseDesc.vPos = XMFLOAT4{ 0.1f, 0.f, -0.23f, 1.f };
		seNoseDesc.fCX = 0.27f;
		seNoseDesc.fCY = 0.27f;
		seNoseDesc.fCZ = 0.27f;
		seNoseDesc.fSpeedPerSecond = 10.f;
		seNoseDesc.pParentState = &m_iState;
		seNoseDesc.pSocketMatrix = BodySockeMatrix;
		seNoseDesc.eRenderGroup = m_eRenderGroup;
		seNoseDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seNoseDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seNoseDesc.pNoseInfo = &m_MiiInfo.NoseInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiNose"), TEXT("Part_Nose"), &seNoseDesc)))
			return E_FAIL;
	}

	/* Part_Mouth */
	/* 101 ~ 128 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiMouth::SEMIIMOUTH_DESC seMouthDesc = {};
		seMouthDesc.vPos = XMFLOAT4{ 0.f, 0.f, -0.23f, 1.f };
		seMouthDesc.fCX = 0.35f;
		seMouthDesc.fCY = 0.35f;
		seMouthDesc.fCZ = 0.27f;
		seMouthDesc.fSpeedPerSecond = 10.f;
		seMouthDesc.pParentState = &m_iState;
		seMouthDesc.pSocketMatrix = BodySockeMatrix;
		seMouthDesc.eRenderGroup = m_eRenderGroup;
		seMouthDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seMouthDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seMouthDesc.pMouthInfo = &m_MiiInfo.MouthInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiMouth"), TEXT("Part_Mouth"), &seMouthDesc)))
			return E_FAIL;
	}

	return S_OK;

}

HRESULT CSEMii::Bind_ShaderResources()
{
	return S_OK;
}

CSEMii* CSEMii::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMii* pInstance = new CSEMii(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMii Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMii::Clone(void* pArg)
{
	CSEMii* pInstace = new CSEMii(*this);
	if (FAILED(pInstace->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMii Clone Failed");
		Safe_Release(pInstace);
		return nullptr;
	}
	return pInstace;
}

void CSEMii::Free()
{
	CContainerObject::Free();
}