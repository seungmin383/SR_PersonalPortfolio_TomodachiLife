#include "TimeLine_Manager.h"
#include "TimeLine_Player.h"

CTimeLine_Manager::CTimeLine_Manager()
{
}

HRESULT CTimeLine_Manager::Initialize()
{
	m_pTimeLinePlayer = CTimeLine_Player::Create();
	return S_OK;	
}

void CTimeLine_Manager::Update_Priority(_float fDT)
{
	m_pTimeLinePlayer->Update_Priority(fDT);
}

void CTimeLine_Manager::Update(_float fDT)
{
	m_pTimeLinePlayer->Update(fDT);
}

void CTimeLine_Manager::Update_Late(_float fDT)
{
	m_pTimeLinePlayer->Update_Late(fDT);
}

HRESULT CTimeLine_Manager::Add_TimeLine(CTimeLine* pTimeline)
{
	return m_pTimeLinePlayer->Begin_Timeline(pTimeline);
}

CTimeLine_Manager* CTimeLine_Manager::Create()
{
	CTimeLine_Manager* pInstance = new CTimeLine_Manager();
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CTimeLine_Manager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTimeLine_Manager::Free()
{
	Safe_Release(m_pTimeLinePlayer);
	CBase::Free();
}
