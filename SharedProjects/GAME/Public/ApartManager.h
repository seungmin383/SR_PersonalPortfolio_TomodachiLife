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
	APARTINFO_DESC& Get_ApartInfo() { return m_ApartInfo; }

private:
	APARTINFO_DESC m_ApartInfo = {};

public:
	virtual void Free()override;
};

NS_END