#pragma once
#include "Game_Defines.h"
#include "Base.h"
#include "ApartInformation.h"

NS_BEGIN(Game)

class CApartManager final : public CBase
{
	DECLARE_SINGLETON(CApartManager)

private:
	CApartManager();
	virtual ~CApartManager() = default;

public:
	HRESULT	Save_ApartData();
	HRESULT Load_ApartData();

public:
	/* mii */
	const _wstring& Get_ApartInfo_MiiName(uint32_t iIndex) const { return m_ApartInfo.strMiiNames[iIndex]; }
	void Add_ApartInfo_NewMii(const _wstring& strNickName) { m_ApartInfo.strMiiNames.push_back(strNickName); }
	void Set_ApartInfo_MiiName(uint32_t iIndex, const _wstring& strNickName) { m_ApartInfo.strMiiNames[iIndex] = strNickName; }

	uint32_t Get_ApartInfo_NumMii() { return static_cast<uint32_t>(m_ApartInfo.strMiiNames.size());}

	/* money */
	int32_t Get_ApartInfo_Money() { return m_ApartInfo.iMoney; }
	void Set_ApartInfo_Money(uint32_t iMoney) { m_ApartInfo.iMoney = iMoney; }
	int32_t Add_ApartInfo_Money(uint32_t iMoney) {return m_ApartInfo.iMoney += iMoney; }
	int32_t Minus_ApartInfo_Money(uint32_t iMoney) {return  m_ApartInfo.iMoney -= iMoney; }

	/* food inven */
	const vector<uint32_t> Get_FoodInventory() { return m_ApartInfo.iFoodInventory; }
	void Add_FoodInventory(uint32_t iIndex);
	void Minus_FoodInventory(uint32_t iIndex);

	/* 친구되기 이벤트 */
	_bool Get_isBeFriend()const { return m_isBeFriend; }
	void Set_isBeFriend(_bool is) { m_isBeFriend = is; }

	uint32_t Get_SrcMiiIndex()const { return m_iSrcMiiIndex; }
	uint32_t Get_DstMiiIndex()const { return m_iDstMiiIndex; }

	void Set_SrcMiiIndex(uint32_t i){ m_iSrcMiiIndex = i; }
	void Set_DstMiiIndex(uint32_t i){ m_iDstMiiIndex = i; }

	_bool Get_isVisitFriendRoom() const { return m_isVisitFriendRoom; }
	void Set_isVisitFriendRoom(_bool is) { m_isVisitFriendRoom = is; }
	
	_bool Get_isVisitingFriendRoom() const { return m_isVisitingFriendRoom; }
	void Set_isVisitingFriendRoom(_bool is) { m_isVisitingFriendRoom = is; }

	/* 연인 만들기 이벤트 */
	_bool Get_isLove()const { return m_isLove; }
	void Set_isLove(_bool is) { m_isLove = is; }
	_bool Get_isStart_Method()const { return m_isStart_Method; }
	void Set_isStart_Method(_bool is) { m_isStart_Method = is; }
	
	uint32_t Get_iLoveMethod()const { return m_iLoveMethod; }
	void Set_iLoveMethod(uint32_t i) { m_iLoveMethod = i; }
	uint32_t Get_iLovePlace()const { return m_iLovePlace; }
	void Set_iLovePlace(uint32_t i) { m_iLovePlace = i; }
	
	_bool Get_isStartPropose()const { return m_isStartPropose; }
	void Set_isStartPropose(_bool is) { m_isStartPropose = is; }

	_bool Get_isProposing() const { return m_isProposing; }
	void Set_isProposing(_bool is) { m_isProposing = is; }

private:
	APARTINFO_DESC m_ApartInfo = {};
	
	map<int32_t, int32_t>m_FoodInventory;

private:
	_bool m_isBeFriend = { FALSE };

	uint32_t m_iSrcMiiIndex = {};
	uint32_t m_iDstMiiIndex = {};

	_bool m_isVisitFriendRoom = { FALSE };
	_bool m_isVisitingFriendRoom = { FALSE };

private:
	_bool m_isLove = { FALSE };
	_bool m_isStart_Method = { FALSE };
	uint32_t m_iLoveMethod = {};
	uint32_t m_iLovePlace = {};

	_bool m_isStartPropose = { FALSE };
	_bool m_isProposing = { FALSE };

public:
	virtual void Free()override;
};

NS_END