#include "Timer_Manager.h"
#include "Timer.h"

CTimerManager::CTimerManager()
{
}

_float CTimerManager::Get_DeltaTime(const _tchar* _pTimerTag)
{
	CTimer* pTimer = Find_Timer(_pTimerTag);

	if (nullptr == pTimer)
		return 0.f;
	
	return pTimer->Get_DeltaTime();
}

void CTimerManager::Calc_DeltaTime(const _tchar* _pTimerTag)
{
	CTimer* pTimer = Find_Timer(_pTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

HRESULT CTimerManager::Ready_Timer(const _tchar* _pTimerTag)
{
	CTimer* pTimer = Find_Timer(_pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();

	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimer.emplace(_pTimerTag, pTimer);

	return S_OK;
}

CTimer* CTimerManager::Find_Timer(const _tchar* _pTimerTag)
{
	auto iter = m_mapTimer.find(_pTimerTag);

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

CTimerManager* CTimerManager::Create()
{
	return new CTimerManager;
}

void CTimerManager::Free()
{
	for (auto& Pair : m_mapTimer)
		Safe_Release(Pair.second);

	m_mapTimer.clear();

	CBase::Free();
}
