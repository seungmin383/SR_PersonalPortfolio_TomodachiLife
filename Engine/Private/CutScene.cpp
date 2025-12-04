#include "CutScene.h"
#include "TimeLine_Trans.h"

CCutScene::CCutScene()
{
}


HRESULT CCutScene::Play_CutScene(_float fDT)
{
	m_fCurTrackPosition += fDT;

	if (nullptr != m_pCameraTimeLine)
		m_pCameraTimeLine->Update_Timeline(m_fCurTrackPosition, fDT);

	if (!m_pActorTimeLine.empty())
	{
		for (const auto& pTimeline : m_pActorTimeLine)
			pTimeline->Update_Timeline(m_fCurTrackPosition, fDT);
	}
	return S_OK;
}

HRESULT CCutScene::Add_TimeLine_Trans(TIMELINE_TYPE eType, CTimeLine_Trans* pTimeLine)
{
	switch (eType)
	{
	case Engine::TIMELINE_TYPE::TRANS:
		m_pActorTimeLine.push_back(pTimeLine);
		break;
	case Engine::TIMELINE_TYPE::ANIM:
		break;
	case Engine::TIMELINE_TYPE::CAMERA:
		break;
	case Engine::TIMELINE_TYPE::END:
		break;
	default:
		break;
	}
	return S_OK;
}


void CCutScene::Free()
{
	Safe_Release(m_pCameraTimeLine);

	for (auto& pTimeline : m_pActorTimeLine)
		Safe_Release(pTimeline);
	m_pActorTimeLine.clear();

	CBase::Free();
}
