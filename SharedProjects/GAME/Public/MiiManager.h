#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Game)

class CMiiManager final : public CBase
{
	DECLARE_SINGLETON(CMiiManager)

private:
	CMiiManager();
	virtual ~CMiiManager() = default;

public:
	HRESULT Initialize();

public:
	HRESULT Begin_Edit(_bool isMan);
	HRESULT End_Edit();

	HRESULT Save_MiiData(const _wstring& strNickName);
	HRESULT Load_MiiData(__out MIIINFO_DESC& Out, const _wstring& strNickName);

public:
	MIIPROFILE_DESC&		Get_Edit_Profile()		{ return m_EditDesc.MiiProfile; }
	MIIHEADINFO_DESC&		Get_Edit_HeadInfo()		{ return m_EditDesc.HeadInfo; }
	MIIHAIRINFO_DESC&		Get_Edit_HairInfo()		{ return m_EditDesc.HairInfo; }
	MIIEYEBROWINFO_DESC&	Get_Edit_EyebrowInfo()	{ return m_EditDesc.EyebrowInfo; }
	MIIEYESINFO_DESC&		Get_Edit_EyesInfo()		{ return m_EditDesc.EyesInfo; }
	MIINOSEINFO_DESC&		Get_Edit_NoseInfo()		{ return m_EditDesc.NoseInfo; }
	MIIMOUTHINFO_DESC&		Get_Edit_MouthInfo()	{ return m_EditDesc.MouthInfo; }

public:
	MIIINFO_DESC& Get_MiiInfo(uint32_t i) { return m_Mii_Informations[i]; }
	
	const _wstring& Get_CurMiiNickName() const { return m_strCurMiiNickName; }
	void Set_CurMiiNickName(const _wstring& strNickName) { m_strCurMiiNickName = strNickName; }

private:
	vector<MIIINFO_DESC > m_Mii_Informations;

	_wstring m_strCurMiiNickName = {};

	MIIINFO_DESC m_EditDesc ;

	_bool m_isSave = { FALSE };
	_bool m_isEdit = { FALSE };

public:
	virtual void Free()override;
};

NS_END