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
	void Add_ApartInfo_Money(uint32_t iMoney) { m_ApartInfo.iMoney += iMoney; }
	void Minus_ApartInfo_Money(uint32_t iMoney) { m_ApartInfo.iMoney -= iMoney; }
private:
	APARTINFO_DESC m_ApartInfo = {};
	
public:
	virtual void Free()override;
};

NS_END