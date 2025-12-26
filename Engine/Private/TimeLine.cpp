#include "TimeLine.h"
#include "GameInstance.h"

CTimeLine::CTimeLine()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

void CTimeLine::Begin_Timeline()
{

}

void CTimeLine::Update_Priority(_float fDT)
{

}

void CTimeLine::Update(_float fDT)
{

}

int32_t CTimeLine::Update_Late(_float fDT)
{
	return TIMELINE_PLAY;
}

void CTimeLine::End_Timeline()
{

}

void CTimeLine::Free()
{
	Safe_Release(m_pGameInstance);
	CBase::Free();
}
