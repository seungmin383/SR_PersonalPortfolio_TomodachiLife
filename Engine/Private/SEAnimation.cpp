#include "SEAnimation.h"
#include "SEChannel.h"

CSEAnimation::CSEAnimation()
{
}

CSEAnimation::CSEAnimation(const CSEAnimation& rhs)
	:m_strName(rhs.m_strName),
	m_fTickPerSec(rhs.m_fTickPerSec),
	m_fDuration(rhs.m_fDuration),
	// m_Channels(rhs.m_Channels),
	m_iNumChannel(rhs.m_iNumChannel),
	m_ChannelPos(rhs.m_ChannelPos)
{
	m_iNumChannel = static_cast<uint32_t>(rhs.m_Channels.size());

	m_Channels.reserve(m_iNumChannel);
	for (auto* pChannel : rhs.m_Channels)
	{
		m_Channels.push_back(pChannel->Clone());
	}
}

HRESULT CSEAnimation::Initialize(SE_ANIMATION& seAnim)
{
	 m_strName = seAnim.strName;
	 m_fDuration = seAnim.fDuration;
	 m_fTickPerSec = seAnim.fTickPerSec;
	 
	 uint32_t NumChannel = static_cast<uint32_t>(seAnim.Channels.size());
	 for (uint32_t i = 0; i < NumChannel; ++i)
	 {
		 CSEChannel* pChannel = CSEChannel::Create(seAnim.Channels[i]);
		 if (nullptr == pChannel)
			 return E_FAIL;
		 m_Channels.push_back(pChannel);
	 }
	 return S_OK;
}

void CSEAnimation::Update_TransformationMatrix(
	_float& fCurTrackPosition,
	_float fDT,
	_bool isChangeAnim,
	_bool isLoop)
{
	fCurTrackPosition += m_fTickPerSec * fDT;

	if (fCurTrackPosition >= m_fDuration)
	{
		if (false == isLoop)
		{
			return;
		}

		fCurTrackPosition = 0.f;
	}

	m_iNumChannel = static_cast<uint32_t>(m_Channels.size());
	m_ChannelPos.resize(m_iNumChannel);

	for (uint32_t i = 0; i < m_iNumChannel; ++i)
	{
		m_Channels[i]->Update_TransformationMatrix(fCurTrackPosition, m_ChannelPos[i]);
	}
	return;
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
CSEAnimation* CSEAnimation::Clone()
{
	return new CSEAnimation(*this);
}

void CSEAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();

	CBase::Free();
}
