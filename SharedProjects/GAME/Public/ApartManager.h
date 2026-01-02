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

private:
	APARTINFO_DESC m_ApartInfo = {};
	
	map<int32_t, int32_t>m_FoodInventory;

	_bool m_isBeFriend = { FALSE };

public:
	virtual void Free()override;
};

NS_END