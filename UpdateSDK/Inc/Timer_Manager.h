#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CTimer;

class CTimerManager final: public CBase
{
private:
    CTimerManager();
    virtual ~CTimerManager() = default;

public:
    _float          Get_DeltaTime(const _tchar* _pTimerTag);     //타이머를 검색 -> 있다면 반환

public:
    HRESULT         Ready_Timer(const _tchar* _pTimerTag);        //타이머를 검색 -> 없다면 설치
    void            Calc_DeltaTime(const _tchar* _pTimerTag);     //타이머를 검색 -> 있다면 시간을 계산

private:
    CTimer*         Find_Timer(const _tchar* _pTimerTag);

private:
    map<_wstring, CTimer*> m_mapTimer;

public:
    static CTimerManager* Create();
    virtual void    Free();
};

NS_END