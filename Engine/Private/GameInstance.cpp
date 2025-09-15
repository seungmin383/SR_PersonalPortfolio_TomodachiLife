#include "GameInstance.h"
#include "Timer_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize()
{
	m_pTimerManager = CTimerManager::Create();

	if (nullptr == m_pTimerManager)
	{
		MSG_BOX("ErrCode : m_pTimerManager Create Failed");
		Safe_Release(m_pTimerManager);
		return E_FAIL;
	}


	return S_OK;
}

inline _float CGameInstance::TimerManager_Get_TimeDelta(const _tchar* pTimerTag)
{
	return m_pTimerManager->Get_TimeDelta(pTimerTag);
}

inline HRESULT CGameInstance::TimerManager_Ready_Timer(const _tchar* pTimerTag)
{
	return m_pTimerManager->Ready_Timer(pTimerTag);
}

inline void CGameInstance::TimerManager_Calc_TimeDelta(const _tchar* pTimerTag)
{
	m_pTimerManager->Calc_TimeDelta(pTimerTag);
}

void CGameInstance::Free()
{
	Safe_Release(m_pTimerManager);




	CBase::Free();


}
