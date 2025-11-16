#include "SEAnimation.h"
#include "SEChannel.h"

CSEAnimation::CSEAnimation()
{
}

HRESULT CSEAnimation::Initialize(SE_ANIMATION& seAnim)
{
	 m_strName = seAnim.strName;
	 m_fTickPerSec = seAnim.fTickPerSec;
	 m_fDuration = seAnim.fDuration;
	 
	 uint32_t NumChannel = seAnim.Channels.size();
	 for (uint32_t i = 0; i < NumChannel; ++i)
	 {
		 CSEChannel* pChannel = CSEChannel::Create(seAnim.Channels[i]);
		 if (nullptr == pChannel)
			 return E_FAIL;
		 m_Channels.push_back(pChannel);
	 }
	 return S_OK;
}

_bool CSEAnimation::Update_TransformationMatrix(
	_float fDT)
{
	m_fCurTrackPosition += m_fTickPerSec * fDT;
	m_iNumChannel = m_Channels.size();
	m_ChannelPos.resize(m_iNumChannel);

	for (uint32_t i = 0; i < m_iNumChannel; ++i)
	{
		CHANNEL_POS chPos = {};
		m_Channels[i]->Update_TransformationMatrix(
			m_fCurTrackPosition,
			m_ChannelPos[i]);
	}
	return false;
}

CSEAnimation* CSEAnimation::Create(SE_ANIMATION& seAnim)
{
	CSEAnimation* pInstance = new CSEAnimation;
	if (FAILED(pInstance->Initialize(seAnim)))
	{
		MSG_BOX("ErrCode : SEAnimation Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();

	CBase::Free();
}
