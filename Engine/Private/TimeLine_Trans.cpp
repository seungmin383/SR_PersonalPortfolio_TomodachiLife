#include "TimeLine_Trans.h"
#include "GameInstance.h"

CTimeLine_Trans::CTimeLine_Trans()
{
}

HRESULT CTimeLine_Trans::Initialize(TIMELINE_TRANS_DESC* pDesc)
{
	m_pOwner = pDesc->pOwner;

	return S_OK;
}

void CTimeLine_Trans::Update_Timeline(_float fTrackPosition, _float fDT)
{
	Calc_CutFrameIndex(fTrackPosition);

	if (m_iCutFrameIndex != m_iPreFrameIndex)
	{
		m_pOwner->Get_TransformCom()->Set_Target(m_CutFrames[m_iCutFrameIndex].vPosition);
	}
	m_pOwner->Get_TransformCom()->Go_Auto(fDT);
}

void CTimeLine_Trans::Add_CuTFrame(const CUTFRAME& CutFrame)
{
	m_CutFrames.push_back(CutFrame);
}

void CTimeLine_Trans::Calc_CutFrameIndex(_float fTrackPosition)
{
	m_iPreFrameIndex = m_iCutFrameIndex;

	if (m_CutFrames.empty())
		return;

	if (fTrackPosition <= m_CutFrames.front().fTrackPosition)
	{
		m_iCutFrameIndex = 0;
		return;
	}

	if (fTrackPosition >= m_CutFrames.back().fTrackPosition)
	{
		m_iCutFrameIndex = static_cast<uint32_t>(m_CutFrames.size() - 1);
		return;
	}

	while (m_iCutFrameIndex > 0 && fTrackPosition < m_CutFrames[m_iCutFrameIndex].fTrackPosition)
		--m_iCutFrameIndex;

	while (m_iCutFrameIndex < static_cast<uint32_t>(m_CutFrames.size() - 1) &&
		fTrackPosition >= m_CutFrames[m_iCutFrameIndex+1].fTrackPosition)
		++m_iCutFrameIndex;
}

CTimeLine_Trans* CTimeLine_Trans::Create(TIMELINE_TRANS_DESC* pDesc)
{
	CTimeLine_Trans* pInstance = new CTimeLine_Trans();
	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("ErrCode : CTimeLine_Trans Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTimeLine_Trans::Free()
{
	CBase::Free();
}
