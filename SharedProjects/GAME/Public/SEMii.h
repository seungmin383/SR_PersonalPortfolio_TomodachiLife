#pragma once
#include "Game_Defines.h"
#include "ContainerObject.h"
#include "GameDirector.h"

NS_BEGIN(Engine)
class CBrain;
NS_END

NS_BEGIN(Game)

class CSEMiiBody;

class CSEMii final : public CContainerObject
{
public:
	enum STATE
	{
		IDLE			= 1u << 0,
		RECEIVE_FOOD	= 1u << 1, /* 이벤트가 시작했는지 확인용*/
		EATING_FOOD		= 1u << 2,
		ATE_FOOD		= 1u << 3,	/* 음식 다 먹음 */
		FOOD_BEST		= 1u << 4,
		FOOD_GOOD		= 1u << 5,
		FOOD_NOTBAD		= 1u << 6,
		FOOD_SOSO		= 1u << 7,
		FOOD_BAD		= 1u << 8,
		FOOD_WORST		= 1u << 9,
		FIN_FOODEVENT	= 1u << 10,
		RECEIVED_FOOD	= 1u << 11,	/* 주민 손에 올라왔는지 */
		TALKING			= 1u << 12,
		BE_FRIEND		= 1u << 13,
		WALKING			= 1u << 14,
		CHANGE_STATE	= 1u << 15,
		BANTER			= 1u << 16,
		HAPPY			= 1u << 17,
		LOVE			= 1u << 18,
		HUNGRY			= 1u << 19,
		CALM			= 1u << 20,
		NERVOUS			= 1u << 21,
		PROPOSE			= 1u << 22,
		THANK			= 1u << 23,
		SAD				= 1u << 24,
		IDLE_HAPPY		= 1u << 25,
	};


public:
	typedef struct tagMiiDesc : public CContainerObject::CONTAINEROBJECT_DESC
	{
		_wstring strNickName;
		uint32_t iIndex;

		_wstring strInitAnimTag = {};

		XMFLOAT3 vPreRotateAngle = {};

	}MII_DESC;

private:
	CSEMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEMii(const CSEMii& rhs);
	virtual ~CSEMii() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	void Initialize_FriendList();

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	/* 플래그들을 받아오기 */
	void Observe();

	/* 받아온 플래그를 이용해서 상태 만들기 */
	void Thinking(_float fDT);

	void Thinking_About_ReceiveFood();
	void Thinking_About_BeFriend(_float fDT);
	void Thinking_About_Love(_float fDT);
	void Thinking_About_Hungry(_float fDT);

	/* 상태에 따른 명령 내리기 */
	void Acting();

	void Acting_For_ReceivedFood();
	void Acting_For_BeFriend();
	void Acting_For_Love();

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

private:
	CGameDirector::GAME_DIRECTION m_eDirection = { CGameDirector::GAME_DIRECTION::END };
	CGameDirector::GAME_DIRECTION m_ePreDirection = { CGameDirector::GAME_DIRECTION::END };

private:
	CSEMiiBody* m_pBody = { nullptr };

	XMFLOAT3 m_vPreRotateAngle = {};

private:
	int32_t m_iState					= {};
	int32_t m_iClothState				= {};
	_bool	m_isChangeState				= { FALSE };

	MIIINFO_DESC m_MiiInfo				= {};

	_wstring m_strNickName				= {};
	uint32_t m_iIndex					= {};

	vector<int32_t> m_FoodFlavorFlags;

private:
	_bool m_isFin_Macro					= { FALSE };

	_bool m_isStart_ReceiveFood_Event	= { FALSE };

	_bool m_isReceivedFood				= { FALSE };
	_bool m_isEatingFood				= { FALSE };
	_bool m_isFin_EatingFood			= { FALSE };
	_bool m_isReactingTaste				= { FALSE };

	_bool m_isLevelUp					= { FALSE };
	_bool m_isFullnessUp				= { FALSE };

private:
	uint32_t m_iWantToBeFriendIndex		= {};
	_wstring m_strWantToBeFriendName	= {};

	_float m_fTalkingTimer				= {};
	_bool m_isFin_FirstTalk				= { FALSE };

	_float m_fTalkingTimer_2			= {};

	_bool m_isVisitFriendRoom			= { FALSE };
	_bool m_isPreVisitingFriendRoom		= { FALSE };
	_bool m_isPreProposing				= { FALSE };

	_float m_fWalkingTimer				= {};
	_bool m_isStartBanter				= {FALSE};
	_float m_isWaitingTImer				= { FALSE };

	_float m_fComeBackAndWaitTimer		= {};
	_float m_fComeBackAndTalkingTimer	= {};
	_bool m_isComeBackRoom				= { FALSE };

	_bool m_isFin_BeFriendEvent			= { FALSE };

private:
	uint32_t m_iLoveMiiIndex			= {};
	_bool m_isLoveEnter					= { FALSE };
	_bool m_isFin_Shy					= { FALSE };

	_float m_fTalkingTimer_3			= {};
	_float m_fTalkingTimer_4			= {};

	_float m_fCameraLookTimer			= {};
	_float m_fWalkingTimer_2			= {};
	_float m_fWalkingTimer_3			= {};
	_float m_fWaitingTimer				= {};
	_float m_fWaitingTimer_1			= {};
	_float m_fWaitingTimer_2			= {};

	_float m_fTalkingTimer_5			= {};

	_float m_fEndTimer					= {};
	_float m_fEndTimer_1				= {};
	_bool m_isStartEndEvent				= { FALSE };

	_float m_fSadTimer					= {};

	_bool m_isFin_LoveEvent				= { FALSE };

public:
	static CSEMii* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END