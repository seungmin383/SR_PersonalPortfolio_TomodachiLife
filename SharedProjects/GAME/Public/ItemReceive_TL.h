#pragma once
#include "Game_Defines.h"
#include "TimeLine.h"

NS_BEGIN(Game)

class CItemReceive_TL final : public CTimeLine
{
private:
	CItemReceive_TL();
	virtual ~CItemReceive_TL() = default;

public:
	static CItemReceive_TL* Create();
	virtual void Free()override;
};

NS_END