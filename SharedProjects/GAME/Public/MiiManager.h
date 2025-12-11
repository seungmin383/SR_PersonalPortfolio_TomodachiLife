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
	void Begin_Edit(MIIINFO_DESC& CurMiiDesc);
	HRESULT Save_MiiData(const _wstring& strFilePath);
	HRESULT Load_MiiData(__out MIIINFO_DESC& Out, const _wstring& strFilePath);

public:
	MIIPROFILE_DESC&		Get_Profile()		{ return m_EditDesc.MiiProfile; }
	MIIHEADINFO_DESC&		Get_HeadInfo()		{ return m_EditDesc.HeadInfo; }
	MIIHAIRINFO_DESC&		Get_HairInfo()		{ return m_EditDesc.HairInfo; }
	MIIEYEBROWINFO_DESC&	Get_EyebrowInfo()	{ return m_EditDesc.EyebrowInfo; }
	MIIEYESINFO_DESC&		Get_EyesInfo()		{ return m_EditDesc.EyesInfo; }
	MIINOSEINFO_DESC&		Get_NoseInfo()		{ return m_EditDesc.NoseInfo; }
	MIIMOUTHINFO_DESC&		Get_MouthInfo()		{ return m_EditDesc.MouthInfo; }

public:
	_bool Get_EditFin() const { return m_isEditFin; }
	void  Set_EditFin(_bool isEditFin) { m_isEditFin = isEditFin; }

private:
	vector<MIIINFO_DESC > m_Mii_Informations;

	MIIINFO_DESC m_EditDesc ;
	_bool m_isSave = { FALSE };
	_bool m_isEditFin = { FALSE };

public:
	virtual void Free()override;
};

NS_END