#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CTimeLine_Player;
class CTimeLine;
class CTimeLine_Manager final : public CBase
{
private:
	CTimeLine_Manager();
	virtual ~CTimeLine_Manager() = default;

public:
	HRESULT Initialize();

	void Update_Priority(_float fDT);
	void Update(_float fDT);
	void Update_Late(_float fDT);

	HRESULT	Add_TimeLine(CTimeLine* pTimeline);

private:
	CTimeLine_Player* m_pTimeLinePlayer = { nullptr };


public:
	static CTimeLine_Manager* Create();
	virtual void Free()override;

};

NS_END