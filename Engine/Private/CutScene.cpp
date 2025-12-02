#include "CutScene.h"

CCutScene::CCutScene()
{
}

void CCutScene::Begin_CutScene()
{
	m_isPlaying = TRUE;
}

void CCutScene::Update_CutScene(_float fDT)
{
	if(m_isPlaying)
		m_fCurTrackPosition += fDT;

}

void CCutScene::End_CutScene()
{
	m_isPlaying = FALSE;
}

void CCutScene::Free()
{
	CBase::Free();
}
