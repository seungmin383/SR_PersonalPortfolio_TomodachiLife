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
#include "ApartManager.h"
#include "FoodItem_Texture.h"
#include "Food_Item.h"
#include "Engine_FlagHelper.h"
#include "NarrationManager.h"

#include "Place_Loading.h"

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
	m_vPreRotateAngle = pDesc->vPreRotateAngle;

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

	Initialize_FriendList();

	return S_OK;
}

void CSEMii::Initialize_FriendList()
{	/* 전체 주민 수를 가져옴 */
	uint32_t iNumMii = CApartManager::GetInstance()->Get_ApartInfo_NumMii();

	/* 나랑 친구인 mii 인덱스들을 가져옴 */
	const auto& iArr = CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.iFriendIndexList;
	_bool isMan = CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.isMan;

	/* 나랑 친구가 아닌 애들을 저장할꺼임*/
	vector<uint32_t>NotFriendList;

	/* 나랑 친구고 이성인 애들 저장할거임 */
	vector<uint32_t> LoveList;

	/* 모든 주민 순회 */
	for (uint32_t i = 0; i < iNumMii; ++i)
	{
		_bool isFriend = { FALSE };

		/* 나면 빼기 */
		if (CMiiManager::GetInstance()->Get_CurMiiIndex() == i)
			continue;

		/* 나랑 친구인 애들 인덱스 j */
		for (uint32_t j : iArr)
		{
			if (j == i)
			{
				isFriend = TRUE;
				break;
			}
		}

		if (!isFriend)
			NotFriendList.push_back(i);
	}

	for (uint32_t j : iArr)
	{
		/* 성별이 다르다면 */
		if (isMan != CMiiManager::GetInstance()->Get_MiiInfo(j).MiiProfile.isMan)
			LoveList.push_back(j);
	}
	
	if (!LoveList.empty() && !CApartManager::GetInstance()->Get_isProposing())
	{
		m_iLoveMiiIndex = LoveList.front();
		CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iNeeds = static_cast<uint32_t>(NEEDS::LOVE);
	}


	uint32_t iNumNotFriend = static_cast<uint32_t>(NotFriendList.size());
	if (iNumNotFriend <= 0)
		return;

	m_iWantToBeFriendIndex = NotFriendList[ m_pGameInstance->Real_Random(0, iNumNotFriend - 1)];
	m_strWantToBeFriendName = StrToWstr(CMiiManager::GetInstance()->Get_MiiInfo(m_iWantToBeFriendIndex).MiiProfile.strNickName);
}

int32_t CSEMii::Update_Priority(_float fDT)
{
	Observe();
	Thinking(fDT);



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
	Clear_Flag(m_iState);

	if (m_eDirection != m_ePreDirection)
	{
		Add_Flag(m_iState, STATE::CHANGE_STATE);
		m_isChangeState = TRUE;
	}
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
	else if (m_eDirection == CGameDirector::GAME_DIRECTION::BE_FRIEND)
	{

	}
	else if (m_eDirection == CGameDirector::GAME_DIRECTION::HUNGRY)
	{

	}
	else if (m_eDirection == CGameDirector::GAME_DIRECTION::LOVE)
	{

	}

	if (CApartManager::GetInstance()->Get_isBuying())
	{
		Add_Flag(m_iState, STATE::THANK);
		Add_Flag(m_iState, STATE::CHANGE_STATE);
		CApartManager::GetInstance()->Set_isBuying(FALSE);
	}
}

void CSEMii::Thinking(_float fDT)
{
	if (m_isChangeState)
	{
		m_isLevelUp = FALSE;
		m_isFullnessUp = FALSE;
		m_fTalkingTimer = 0.f;
	}

	if (CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.iNeeds == static_cast<uint32_t>(NEEDS::LOVE) && 
		!m_isLoveEnter && 
		!CApartManager::GetInstance()->Get_isProposing())
	{
		Add_Flag(m_iState, STATE::LOVE);
		Add_Flag(m_iState, STATE::CHANGE_STATE);
	}

	/* 전역 스테이트로 1차 내부 스테이트를 설정 */
	switch (m_eDirection)
	{
	case Game::CGameDirector::GAME_DIRECTION::NO_EVENT:
		Add_Flag(m_iState, STATE::IDLE);
		break;


	case Game::CGameDirector::GAME_DIRECTION::GIVE_ITEM:
		break;	
	
	
	case Game::CGameDirector::GAME_DIRECTION::HUNGRY:
		Add_Flag(m_iState, STATE::HUNGRY);
		break;


	case Game::CGameDirector::GAME_DIRECTION::GIVE_FOOD:
		Add_Flag(m_iState, STATE::RECEIVE_FOOD);
		break;


	case Game::CGameDirector::GAME_DIRECTION::BE_FRIEND:
		Add_Flag(m_iState, STATE::BE_FRIEND);
		break;


	case Game::CGameDirector::GAME_DIRECTION::LOVE:
		Add_Flag(m_iState, STATE::LOVE);
		break;

	default:
		break;
	}
	
	/* 내 정보를 이용해서 2차 내부 스테이트를 추가 */
	if (m_eDirection == CGameDirector::GAME_DIRECTION::GIVE_FOOD)
		Thinking_About_ReceiveFood();

	else if (m_eDirection == CGameDirector::GAME_DIRECTION::BE_FRIEND)
		Thinking_About_BeFriend(fDT);

	else if (m_eDirection == CGameDirector::GAME_DIRECTION::LOVE)
		Thinking_About_Love(fDT);

	else if (m_eDirection == CGameDirector::GAME_DIRECTION::HUNGRY)
		Thinking_About_Hungry(fDT);

	/* 파트 오브젝트의 값을 이용해서 3차 내부 스테이트를 추가 */

	
}

void CSEMii::Thinking_About_ReceiveFood()
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
		Add_Flag(m_iState, (STATE::RECEIVED_FOOD|STATE::TALKING));

	if (m_isFin_EatingFood || m_isReactingTaste)
		Add_Flag(m_iState, STATE::ATE_FOOD);

	if (m_isEatingFood)
		Add_Flag(m_iState, STATE::EATING_FOOD);

}

void CSEMii::Thinking_About_BeFriend(_float fDT)
{
	m_fTalkingTimer += fDT;
	_bool isVisit = CApartManager::GetInstance()->Get_isVisitFriendRoom();
	_bool isVisiting = CApartManager::GetInstance()->Get_isVisitingFriendRoom();

	if (!isVisit)
	{
		if (m_fTalkingTimer < 4.f)
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{1.f, 1.f, 1.f, 1.f});

			CNarrationManager::GetInstance()->Set_NarrationText(L"'" + m_strWantToBeFriendName + L"'"
 			+ L"이/가 저랑 친구가 되고 싶다는데요...");

			Add_Flag(m_iState, STATE::TALKING);
		}
		else if(m_fTalkingTimer >= 4.f && !CApartManager::GetInstance()->Get_isBeFriend())
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(PLACEID::ROOM), TEXT("LayerTag_UI_BeFriend_Button"));
		}
		else if (m_fTalkingTimer >= 4.f && CApartManager::GetInstance()->Get_isBeFriend() && m_fTalkingTimer_2 < 4.f)
		{
			m_fTalkingTimer_2 += fDT;
			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("그럼 얘기를 좀 해 보고 올게요."));

			Add_Flag(m_iState, STATE::TALKING);
		}
		else if(!CApartManager::GetInstance()->Get_isVisitFriendRoom())
		{
			CApartManager::GetInstance()->Set_isVisitFriendRoom(TRUE);
			CApartManager::GetInstance()->Set_isVisitingFriendRoom(TRUE);

			CApartManager::GetInstance()->Set_DstMiiIndex(m_iIndex);
			CApartManager::GetInstance()->Set_SrcMiiIndex(m_iWantToBeFriendIndex);

			m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(PLACEID::LOADING), 
				CPlace_Loading::Create(m_pDevice, m_pContext, PLACEID::EVENT_BEFRIEND));
		}
	}
	else if (isVisiting)
	{
		if(m_isPreVisitingFriendRoom != isVisiting)
			Add_Flag(m_iState, STATE::CHANGE_STATE);
		


		m_fWalkingTimer += fDT;

		/* 방에 찾아간 애 */
		if (m_iIndex == CApartManager::GetInstance()->Get_DstMiiIndex())
		{
			/* 3초동안 걸어가고 */
			if (m_fWalkingTimer < 3.f)
			{
				Add_Flag(m_iState, STATE::WALKING);
				m_pTransformCom->Go_Backward(fDT);
			}
			/* 3초가 지났을 떄 */
			else
			{
				/* 대화 시작을 아직 안했다면 */
				if (!m_isStartBanter)
				{
					/* flag에 Banter 추가해주고 -> Begin Macro 돌려줌 */
					m_isStartBanter = TRUE;
					Add_Flag(m_iState, STATE::CHANGE_STATE);
					Add_Flag(m_iState, STATE::BANTER);
				}
				else if (m_isStartBanter && m_pBody->Get_isFin_Banter() )
				{
					m_isWaitingTImer += fDT;
					Add_Flag(m_iState, STATE::TALKING);

					if (m_isWaitingTImer > 3.f)
					{

						CApartManager::GetInstance()->Set_isVisitingFriendRoom(FALSE);

						m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(PLACEID::LOADING),
							CPlace_Loading::Create(m_pDevice, m_pContext, PLACEID::ROOM));

					}
				}
			}
		}

		/* 방에 있던 애 */
		else if(m_iIndex == CApartManager::GetInstance()->Get_SrcMiiIndex())
		{
			m_pTransformCom->Ratate(0.f, XMConvertToRadians(180.f), 0.f);
			//m_pTransformCom->TurnY(XMConvertToRadians(180.f));

			if (m_fWalkingTimer < 3.f)
			{
			}
			else
			{
				/* 똑같이 Begin Macro용*/
				if (!m_isStartBanter)
				{
					m_isStartBanter = TRUE;
					Add_Flag(m_iState, STATE::CHANGE_STATE);
					Add_Flag(m_iState, STATE::BANTER);
				}
			}
		}

	}
	else
	{
		m_fComeBackAndWaitTimer += fDT;

		if (m_fComeBackAndWaitTimer < 4.f)
		{
			if (!m_isComeBackRoom)
			{
				m_isComeBackRoom = TRUE;
				Add_Flag(m_iState, STATE::CHANGE_STATE);
			}

			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("잘 풀렸습니다!"));
			Add_Flag(m_iState, STATE::HAPPY);
		}
		else
		{
			m_fComeBackAndTalkingTimer += fDT;

			if (m_fComeBackAndTalkingTimer < 4.f)
			{
				CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
				CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

				CNarrationManager::GetInstance()->Set_NarrationText(TEXT("죽이 잘 맞을 것 같아서 친구가 되기로 했어요."));
			
				Add_Flag(m_iState, STATE::HAPPY);
				Add_Flag(m_iState, STATE::TALKING);
			}
			else
			{
				if (!m_isFin_BeFriendEvent)
				{
					Add_Flag(m_iState, STATE::CHANGE_STATE);
					m_isFin_BeFriendEvent = TRUE;
				}

				CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::NO_EVENT);

				CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
				CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iNeeds = static_cast<uint32_t>(NEEDS::NOTHING);
				CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iFriendIndexList.push_back(CApartManager::GetInstance()->Get_SrcMiiIndex());
			}
		}
	}


	m_isPreVisitingFriendRoom = isVisiting;
}

void CSEMii::Thinking_About_Love(_float fDT)
{
	m_fTalkingTimer += fDT;
	_bool isStartPropose = CApartManager::GetInstance()->Get_isStartPropose();
	_bool isProposing = CApartManager::GetInstance()->Get_isProposing();
	_bool isFinPropose = CApartManager::GetInstance()->Get_isFin_Proposing();

	if (m_fEndTimer_1 > 3.f)
	{
		CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
		CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iNeeds = static_cast<uint32_t>(NEEDS::NOTHING);
		CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::NO_EVENT);
		Add_Flag(m_iState, STATE::IDLE);

		m_isFin_LoveEvent = TRUE;

		return;
	}

	m_isLoveEnter = TRUE;

	if (!isStartPropose)
	{
		if (m_fTalkingTimer < 1.f)
		{
			static _bool isFirstEnter = { FALSE };
			if (!isFirstEnter)
			{
				Add_Flag(m_iState, STATE::CHANGE_STATE);
				Remove_Flag(m_iState, STATE::LOVE);
				isFirstEnter = TRUE;
				m_isFin_Shy = TRUE;
			}
			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

			CNarrationManager::GetInstance()->Set_NarrationText(L"사실은 '" + StrToWstr(CMiiManager::GetInstance()->Get_MiiInfo(m_iLoveMiiIndex).MiiProfile.strNickName) + L"'"
				+ L" 을/를 좋아해요. 고백을 할까 하는데요...");

			Add_Flag(m_iState, STATE::TALKING);
		}
		else if (m_fTalkingTimer >= 1.f && !CApartManager::GetInstance()->Get_isLove())
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(PLACEID::ROOM), TEXT("LayerTag_UI_Love_Button"));
		}
		else if (m_fTalkingTimer >= 1.f && CApartManager::GetInstance()->Get_isLove() && m_fTalkingTimer_2 < 1.f)
		{
			m_fTalkingTimer_2 += fDT;
			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("어떤 식으로 고백하는 게 좋을까요?"));

			Add_Flag(m_iState, STATE::TALKING);
		}
		else if (m_fTalkingTimer_2 >= 1.f && CApartManager::GetInstance()->Get_iLoveMethod() == 0)
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(PLACEID::ROOM), TEXT("LayerTag_UI_LoveMethod_Button"));
			m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(PLACEID::ROOM), TEXT("LayerTag_UI_LoveMethod_Button1"));
		}

		else if (m_fTalkingTimer_2 >= 1.f && CApartManager::GetInstance()->Get_iLoveMethod() != 0 && m_fTalkingTimer_3 < 1.f)
		{
			m_fTalkingTimer_3 += fDT;
			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("고백할 장소로는 어디가 좋을까요?"));

			Add_Flag(m_iState, STATE::TALKING);
		}
		else if (m_fTalkingTimer_3 >= 1.f && CApartManager::GetInstance()->Get_iLovePlace() == 0)
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(PLACEID::ROOM), TEXT("LayerTag_UI_LovePlace_Button"));
			m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(PLACEID::ROOM), TEXT("LayerTag_UI_LovePlace_Button1"));
		}

		else if (m_fTalkingTimer_3 >= 1.f && CApartManager::GetInstance()->Get_iLovePlace() != 0 && m_fTalkingTimer_4 < 1.f)
		{
			m_fTalkingTimer_4 += fDT;

			CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
			CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

			CNarrationManager::GetInstance()->Set_NarrationText(TEXT("좋아요. 그럼 용기 내서 고백하고 올게요!"));

			Add_Flag(m_iState, STATE::TALKING);
		}

		else if (!CApartManager::GetInstance()->Get_isProposing())
		{

			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);

			CApartManager::GetInstance()->Set_isProposing(TRUE);
			CApartManager::GetInstance()->Set_isStartPropose(TRUE);

			CApartManager::GetInstance()->Set_DstMiiIndex(m_iIndex);
			CApartManager::GetInstance()->Set_SrcMiiIndex(m_iLoveMiiIndex);

			m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(PLACEID::LOADING),
				CPlace_Loading::Create(m_pDevice, m_pContext, PLACEID::EVENT_LOVE));
		}
	}
	else if (isProposing && !isFinPropose)
	{
		if (m_isPreProposing != isProposing)
			Add_Flag(m_iState, STATE::CHANGE_STATE);

		/* 고백 하는 애 */
		if (m_iIndex == CApartManager::GetInstance()->Get_DstMiiIndex())
		{
			/* 이벤트 1이 실행중이라면 */
			if (!CApartManager::GetInstance()->Get_isFinEvent1())
			{
				m_fCameraLookTimer += fDT;

				/* 카메라가 얘를 바라봄 */
				if (m_fCameraLookTimer < 3.f)
				{
					static _bool is = { FALSE };
					m_pTransformCom->Go_Backward(fDT);

					if (!is)
					{
						Add_Flag(m_iState, STATE::CHANGE_STATE);
						Add_Flag(m_iState, STATE::WALKING);
						is = TRUE;
					}
				}
				else if (m_fCameraLookTimer >= 3.f && m_fCameraLookTimer < 4.5f)
				{
					static _bool is = { FALSE };
					if (!is)
					{
						Add_Flag(m_iState, STATE::CHANGE_STATE);
						Add_Flag(m_iState, STATE::CALM);
						is = TRUE;
					}

					Add_Flag(m_iState, STATE::NERVOUS);

					m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 53.f, 5.f, -5.f, 1.f });
					m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 30.f, 5.f, 30.f, 1.f }, XMConvertToRadians(45.f), 0.1f);

				}
				else
				{
					CApartManager::GetInstance()->Set_isFinEvent1(TRUE);
				}
			}

			/* 이벤트 3가 끝났다면 */
			else if (CApartManager::GetInstance()->Get_isFinEvent3() && m_fWaitingTimer_1 < 5.f)
			{
				m_fWaitingTimer_1 += fDT;

				static _bool is = { FALSE };
				if (!is)
				{
					Add_Flag(m_iState, STATE::CHANGE_STATE);
					Add_Flag(m_iState, STATE::PROPOSE);

					is = TRUE;
				}


				CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
				CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

				CNarrationManager::GetInstance()->Set_NarrationText(TEXT("내가 없는 세상을 한번 상상해 봐요. 혹시라도 허전하다면 당신에겐 내가 필요해요."));

				Add_Flag(m_iState, STATE::TALKING);
			}
			else if (m_fWaitingTimer_1 >= 5.f && m_fWaitingTimer_2 < 1.f)
			{
				m_fWaitingTimer_2 += fDT;

				CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			}
			else if (m_fWaitingTimer_2 >= 1.f)
			{
				CApartManager::GetInstance()->Set_isFinEvent4(TRUE);
				m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 50.f, 5.f, 5.f, 1.f });
				m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ -200.f, 5.f, -10000.f, 1.f }, XMConvertToRadians(45.f), 0.1f);
			}
		}

		/* 고백 당하는 애 */
		else if (m_iIndex == CApartManager::GetInstance()->Get_SrcMiiIndex())
		{
			m_pTransformCom->Ratate(0.f, XMConvertToRadians(220.f), 0.f);

			/* 이벤트 1이 끝났다면 / 이벤트 2가 실행중이라면 */
			if (CApartManager::GetInstance()->Get_isFinEvent1() && !CApartManager::GetInstance()->Get_isFinEvent2())
			{
				if (m_fWalkingTimer_2 < 2.f)
				{
					m_fWalkingTimer_2 += fDT;

					static _bool is = { FALSE };
					if (!is)
					{
						Add_Flag(m_iState, STATE::CHANGE_STATE);
						Add_Flag(m_iState, STATE::WALKING);

						m_pTransformCom->Set_State(TRANSFORM_STATE::POS, XMFLOAT4{ 30.f, 0.f, -40.f, 1.f });
						is = TRUE;
					}

					m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ -30.f, 5.f, -60.f, 1.f }, XMConvertToRadians(45.f), 0.1f);
					m_pTransformCom->Go_Backward(fDT);
					m_pTransformCom->Go_Left(fDT * 1.1f);
				}

				else if (m_fWalkingTimer_2 >= 2.f)
				{
					CApartManager::GetInstance()->Set_isFinEvent2(TRUE);
				}

			}
			/* 이벤트 2가 끝났다면 */
			if (CApartManager::GetInstance()->Get_isFinEvent2() && m_fWalkingTimer_3 < 3.f)
			{
				m_pTransformCom->Ratate(0.f, XMConvertToRadians(180.f), 0.f);
				m_fWalkingTimer_3 += fDT;

				static _bool is = { FALSE };
				if (!is)
				{
					is = TRUE;
					Add_Flag(m_iState, STATE::CHANGE_STATE);
					Add_Flag(m_iState, STATE::WALKING);

					m_pTransformCom->Set_State(TRANSFORM_STATE::POS, XMFLOAT4{ 50.f, 0.f, -6.f, 1.f });
				}

				m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
				m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 70.f, 10.6f, -20.f, 1.f });
				m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 50.f, 5.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);

				m_pTransformCom->Go_Backward(fDT);
			}
			else if (CApartManager::GetInstance()->Get_isFinEvent2() && m_fWalkingTimer_3 >= 3.f && m_fWaitingTimer < 1.f)
			{
				m_pTransformCom->Ratate(0.f, XMConvertToRadians(180.f), 0.f);
				m_fWaitingTimer += fDT;

				static _bool is = { FALSE };
				if (!is)
				{
					is = TRUE;
					Add_Flag(m_iState, STATE::CHANGE_STATE);
					Add_Flag(m_iState, STATE::CALM);
				}
			}
			else if (CApartManager::GetInstance()->Get_isFinEvent2() && m_fWalkingTimer_3 >= 3.f && m_fWaitingTimer >= 1.f && !CApartManager::GetInstance()->Get_isFinEvent4())
			{
				m_pTransformCom->Ratate(0.f, XMConvertToRadians(160.f), 0.f);
				m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 60.f, 5.f, -10.f, 1.f });
				m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 30.f, 5.f, 30.f, 1.f }, XMConvertToRadians(45.f), 1.f);

				CApartManager::GetInstance()->Set_isFinEvent3(TRUE);
			}
			else if (CApartManager::GetInstance()->Get_isFinEvent4())
			{
				m_pTransformCom->Ratate(0.f, XMConvertToRadians(180.f), 0.f);

				int32_t iSrcPersonal = CMiiManager::GetInstance()->Get_MiiInfo(CApartManager::GetInstance()->Get_SrcMiiIndex()).MiiProfile.iPersonal;
				int32_t iDstPersonal = CMiiManager::GetInstance()->Get_MiiInfo(CApartManager::GetInstance()->Get_DstMiiIndex()).MiiProfile.iPersonal;

				/* 서로 다를떄만 1 -> 부호비트가 다르다면 1 -> 음수 / 즉 xor을 해서 양수라면 -> 두개의 부호가 같다 / xor을 해서 음수라면 -> 두개의 부호가 다르다.*/
				/* 부호가 같다 */
				if ((iSrcPersonal ^ iDstPersonal) >= 0)
				{
					m_fTalkingTimer_5 += fDT;
					if (m_fTalkingTimer_5 > 6.f)
					{
						CApartManager::GetInstance()->Set_isFin_Proposing(TRUE);
						CApartManager::GetInstance()->Set_isSuccessPropose(TRUE);
						m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(PLACEID::LOADING),
							CPlace_Loading::Create(m_pDevice, m_pContext, PLACEID::ROOM));
					}
					else if (m_fTalkingTimer_5 > 3.f)
					{
						CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
						CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
						CNarrationManager::GetInstance()->Set_NarrationText(TEXT("좋아요, 잘 부탁해요."));
						Add_Flag(m_iState, STATE::TALKING);
					}
				}
				/* 부호가 다르다 */
				else if ((iSrcPersonal ^ iDstPersonal) < 0)
				{
					m_fTalkingTimer_5 += fDT;

					if (m_fTalkingTimer_5 > 6.f)
					{
						CApartManager::GetInstance()->Set_isFin_Proposing(TRUE);
						CApartManager::GetInstance()->Set_isSuccessPropose(FALSE);
						m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(PLACEID::LOADING),
							CPlace_Loading::Create(m_pDevice, m_pContext, PLACEID::ROOM));
					}
					else if (m_fTalkingTimer_5 > 3.f)
					{
						CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
						CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
						CNarrationManager::GetInstance()->Set_NarrationText(TEXT("미안합니다."));
						Add_Flag(m_iState, STATE::TALKING);
					}
				}

			}

		}

	}
	else if (isFinPropose)
	{
		m_fEndTimer += fDT;

		if (m_fEndTimer > 1.f)
		{
			static _bool is = { FALSE };
			if (!is)
			{
				Add_Flag(m_iState, STATE::CHANGE_STATE);
				is = TRUE;
			}
		}
		if (!CApartManager::GetInstance()->Get_isSuccessPropose())
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			Add_Flag(m_iState, STATE::SAD);

			m_fSadTimer += fDT;


			if (m_fSadTimer > 4.f)
			{
				m_fEndTimer_1 = 100.f;

				CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			}
			else if (m_fEndTimer >= 1.f)
			{
				CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
				CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
				CNarrationManager::GetInstance()->Set_NarrationText(TEXT("잘 안됐나봐요."));

				// CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iFriendIndexList.pop_back();
			}


		}
		else if (CApartManager::GetInstance()->Get_isSuccessPropose())
		{
			CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
			Add_Flag(m_iState, STATE::HAPPY);

			if (m_pBody->Get_isFin_Anim())
			{
				m_isStartEndEvent = TRUE;
				Add_Flag(m_iState, STATE::CHANGE_STATE);
			}

			if (m_isStartEndEvent)
			{
				m_fEndTimer_1 += fDT;

				Add_Flag(m_iState, STATE::IDLE_HAPPY);
				Remove_Flag(m_iState, STATE::HAPPY);

				CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
				CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
				CNarrationManager::GetInstance()->Set_NarrationText(TEXT("성공했습니다."));


				CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iFriendIndexList.clear();
				CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iLoveMiiIndex = CApartManager::GetInstance()->Get_SrcMiiIndex();
			}
		}
	}

	m_isPreProposing = isProposing;
}

void CSEMii::Thinking_About_Hungry(_float fDT)
{
	m_fTalkingTimer += fDT;

	if (m_fTalkingTimer < 2.f)
	{
		CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
		CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

		CNarrationManager::GetInstance()->Set_NarrationText(L"배가 고프네요.");

		Add_Flag(m_iState, STATE::TALKING);
	}
	else
	{
		CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
		CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::NO_EVENT);
	}
}

void CSEMii::Acting()
{
	if (m_eDirection == CGameDirector::GAME_DIRECTION::GIVE_FOOD)
		Acting_For_ReceivedFood();
	else if (m_eDirection == CGameDirector::GAME_DIRECTION::BE_FRIEND)
		Acting_For_BeFriend();
	else if (m_isFin_LoveEvent)
		Acting_For_Love();

	if (CApartManager::GetInstance()->Get_isThankTalking())
	{
		CNarrationManager::GetInstance()->Set_isEnabledNarration(TRUE);
		CNarrationManager::GetInstance()->Set_NarrationColor(XMFLOAT4{ 1.f, 1.f,1.f,1.f });
		CNarrationManager::GetInstance()->Set_NarrationText(TEXT("구입해 주셔서 감사합니다!"));
	}

}

void CSEMii::Acting_For_ReceivedFood()
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

		CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.iNeeds = static_cast<uint32_t>(NEEDS::NOTHING);
	}

	if (m_isStart_ReceiveFood_Event && m_isFin_Macro)
		CNarrationManager::GetInstance()->Set_isEnabledNarration(FALSE);
}

void CSEMii::Acting_For_BeFriend()
{
	if (!m_isLevelUp && m_isFin_BeFriendEvent)
	{
		m_isLevelUp = TRUE;
		CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += 0.3f;
		CApartManager::GetInstance()->Add_ApartInfo_Money(2000);
	}
}

void CSEMii::Acting_For_Love()
{
	if (!m_isLevelUp)
	{
		if (CApartManager::GetInstance()->Get_isSuccessPropose())
		{
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel += 0.3f;
			CApartManager::GetInstance()->Add_ApartInfo_Money(3000);
		}
		else
			CMiiManager::GetInstance()->Edit_CurMiiInfo().MiiProfile.fLevel -= 0.3f;

		m_isLevelUp = TRUE;
	}
}


HRESULT CSEMii::Ready_Components()
{
	return S_OK;
}

HRESULT CSEMii::Ready_PartObjects()
{
	m_MiiInfo = CMiiManager::GetInstance()->Get_MiiInfo(m_iIndex);
	m_iClothState = m_MiiInfo.MiiProfile.iClothState;

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
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_HeadSocketMatrices();

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
		seHeadDesc.pParentState = &m_iState;
		seHeadDesc.pClothState = &m_iClothState;
		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiHead"), TEXT("Part_Head"), &seHeadDesc)))
			return E_FAIL;
	}

	/* Part_Hair */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_HeadSocketMatrices();

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

		seHairDesc.pClothState = &m_iClothState;
		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiHair"), TEXT("Part_Hair"), &seHairDesc)))
			return E_FAIL;
	}

	/* 플레이어가 들 레벨업 아이템 */
	// ProtoTag_Actor_LevelUp_Item
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_LHandSocketMatrices();

		CLevelUp_Item::LEVELUPITEM_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.1f, 0.5f, 1.f };
		Desc.fCX = 0.3f;
		Desc.fCY = 0.3f;
		Desc.fCZ = 0.3f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.eRenderGroup = m_eRenderGroup;
		Desc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		Desc.pSocketMatrix = BodySockeMatrix;

		Desc.pParentClothState = &m_iClothState;
		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_LevelUp_Item"), TEXT("Part_RItem"), &Desc)))
			return E_FAIL;
	}

	/* 플레이어가 들 음식 아이템 */
	// ProtoTag_Actor_Food_Item
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_LHandSocketMatrices();

		CFood_Item::FOODITEM_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, -0.5f, 1.f };
		Desc.fCX = 0.3f;
		Desc.fCY = 0.3f;
		Desc.fCZ = 0.3f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.eRenderGroup = m_eRenderGroup;
		Desc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		Desc.pSocketMatrix = BodySockeMatrix;
		Desc.pParentState = &m_iState;

		Desc.pClothState = &m_iClothState;
		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_Food_Item"), TEXT("Part_RFood"), &Desc)))
			return E_FAIL;
	}

	/* Part_RightEyes */
	/* Part_LeftEyes */
	/* 0 ~ 59 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_HeadSocketMatrices();

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

		seEyesDesc.pClothState = &m_iClothState;
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
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_HeadSocketMatrices();

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

		seEyebrowDesc.pClothState = &m_iClothState;
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
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_HeadSocketMatrices();

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

		seNoseDesc.pClothState = &m_iClothState;
		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiNose"), TEXT("Part_Nose"), &seNoseDesc)))
			return E_FAIL;
	}

	/* Part_Mouth */
	/* 101 ~ 128 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		vector<const XMFLOAT4X4*>* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_HeadSocketMatrices();

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
		seMouthDesc.pClothState = &m_iClothState;
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