#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CTimerManager;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize();

#pragma region TIMER_MANAGER
public:
	_float          TimerManager_Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT         TimerManager_Ready_Timer(const _tchar* pTimerTag);
	void            TimerManager_Calc_TimeDelta(const _tchar* pTimerTag);
#pragma endregion

private:
	CTimerManager* m_pTimerManager = { nullptr };
public:
	virtual void Free()override;
};

NS_END