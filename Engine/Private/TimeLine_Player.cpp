#include "TimeLine_Player.h"
#include "GameInstance.h"
#include "TimeLine.h"

CTimeLine_Player::CTimeLine_Player()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CTimeLine_Player::Begin_Timeline(CTimeLine* pTimeLine)
{
	if (nullptr == pTimeLine)
		return E_FAIL;

	/* 최초로 추가했는지 */
	if (m_pTimeLines.empty())
	{
		m_isFirstEnter = TRUE;
		m_pTimeLines.push(pTimeLine);
	}
	/* 타임라인이 돌고 있을 때 추가했는지 */
	else
	{
		m_pTimeLines.push(pTimeLine);
	}

	return S_OK;
}

HRESULT CTimeLine_Player::Change_Timeline()
{
	if (m_pTimeLines.empty())
	{
		m_isFirstEnter = FALSE;
		m_isTimelineEnd = FALSE;
		m_isPlaying = FALSE;
		return S_OK;
	}

	/* 타임라인이 최초로 시작할 때 */
	if (m_isFirstEnter)
	{
		m_isFirstEnter = FALSE;

		m_pTimeLines.front()->Begin_Timeline();
		m_isPlaying = TRUE;
		m_isTimelineEnd = FALSE;
	}
	/* 타임라인이 다 돌고 다음 걸 시작할 때 */
	else if (m_isTimelineEnd)
	{
		Safe_Release(m_pTimeLines.front());
		m_pTimeLines.pop();

		/* 마지막 타임라인 였을 때 */
		if (m_pTimeLines.empty())
		{
			m_isTimelineEnd = FALSE;
			m_isPlaying = FALSE;
		}
		/* 아직 남은 타임라인이 있을 때 */
		else
		{
			m_isTimelineEnd = FALSE;
			m_isPlaying = TRUE;
			m_pTimeLines.front()->Begin_Timeline();
		}
	}
	/* 아직 타임라인이 돌고 있을 때 */
	else if(m_isPlaying)
	{
		m_isTimelineEnd = FALSE;
	}

	return S_OK;
}

void CTimeLine_Player::Update_Priority(_float fDT)
{
	if (FAILED(Change_Timeline()))
		return;

	if (m_isPlaying && !m_pTimeLines.empty())
		m_pTimeLines.front()->Update_Priority(fDT);
}

void CTimeLine_Player::Update(_float fDT)
{
	if (m_isPlaying && !m_pTimeLines.empty())
		m_pTimeLines.front()->Update(fDT);
}

void CTimeLine_Player::Update_Late(_float fDT)
{
	int32_t iFlag = {};

	if (m_isPlaying && !m_pTimeLines.empty())
		iFlag = m_pTimeLines.front()->Update_Late(fDT);

	if (m_isPlaying && !m_pTimeLines.empty() && (iFlag == TIMELINE_END))
	{
		m_pTimeLines.front()->End_Timeline();
		m_isTimelineEnd = TRUE;
		m_isPlaying = FALSE;
	}
}

CTimeLine_Player* CTimeLine_Player::Create()
{
	return new CTimeLine_Player();
}
void CTimeLine_Player::Free()
{
	while (!m_pTimeLines.empty())
	{
		Safe_Release(m_pTimeLines.front());
		m_pTimeLines.pop();
	}

	Safe_Release(m_pGameInstance);
	CBase::Free();
}
