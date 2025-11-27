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
	void Ready_EditMii(const MIIINFO_DESC& CurMiiDesc);

	MIIHEADINFO_DESC&		Get_HeadInfo() { return m_CurMiiDesc.HeadInfo; }
	MIIHAIRINFO_DESC&		Get_HairInfo(){ return m_CurMiiDesc.HairInfo; }
	MIIEYEBROWINFO_DESC&	Get_EyebrowInfo(){ return m_CurMiiDesc.EyebrowInfo; }
	MIIEYESINFO_DESC&		Get_EyesInfo(){ return m_CurMiiDesc.EyesInfo; }
	MIINOSEINFO_DESC&		Get_NoseInfo(){ return m_CurMiiDesc.NoseInfo; }
	MIIMOUTHINFO_DESC&		Get_MouthInfo(){ return m_CurMiiDesc.MouthInfo; }


private:
	MIIINFO_DESC m_CurMiiDesc ;

public:
	virtual void Free()override;
};

NS_END