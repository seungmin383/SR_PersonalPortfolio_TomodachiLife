#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CGameInstance;
class CTimeLine;
class CTimeLine_Player final : public CBase
{
private:
	CTimeLine_Player();
	virtual ~CTimeLine_Player() = default;

public:
	HRESULT Begin_Timeline(CTimeLine* pTimeLine);
	HRESULT Change_Timeline();

	void Update_Priority(_float fDT);
	void Update(_float fDT);
	void Update_Late(_float fDT);

public:
	_bool Get_IsPlaying() const { return m_isPlaying; }
	_bool Get_IsTimelineEnd() const { return m_isTimelineEnd; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	queue<CTimeLine*> m_pTimeLines;

private:
	/* 최초로 시작했는지 확인용 */
	_bool m_isFirstEnter = { FALSE };

	_bool m_isPlaying = { FALSE };
	_bool m_isTimelineEnd = { TRUE };

public:
	static CTimeLine_Player* Create();
	virtual void Free() override;
};

NS_END