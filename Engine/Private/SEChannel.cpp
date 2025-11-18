#include "SEChannel.h"

CSEChannel::CSEChannel()
{
}

CSEChannel::CSEChannel(const CSEChannel& rhs)
	:m_iBoneIndex(rhs.m_iBoneIndex),
	m_Keyframes(rhs.m_Keyframes),
	m_iCurKeyframeIndex(rhs.m_iCurKeyframeIndex)
{
}

HRESULT CSEChannel::Initialize(SE_CHANNEL& seChannel)
{
	m_iBoneIndex = seChannel.iBoneIndex;
	uint32_t NumKeyframe = static_cast<uint32_t>(seChannel.Keyframes.size());

	for (uint32_t i = 0; i < NumKeyframe; ++i)
	{
		KEYFRAME kf = seChannel.Keyframes[i];
		m_Keyframes.push_back(kf);
	}
	return S_OK;
}

void CSEChannel::Update_TransformationMatrix(
	_float fCurTrackPosition,
	CHANNEL_POS& ChannelPos)
{
	if (0 == fCurTrackPosition)
	{
		m_iCurKeyframeIndex = 0;
	}

	KEYFRAME LastKeyFrame = m_Keyframes.back();

	XMVECTOR vScale = {};
	XMVECTOR vRotation = {};
	XMVECTOR vTranslation = {};

	if (fCurTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}
	else
	{
		while (fCurTrackPosition > m_Keyframes[m_iCurKeyframeIndex + 1].fTrackPosition)
			++m_iCurKeyframeIndex;

		_float fRatio =
			(fCurTrackPosition - m_Keyframes[m_iCurKeyframeIndex].fTrackPosition) /
			(m_Keyframes[m_iCurKeyframeIndex + 1].fTrackPosition - m_Keyframes[m_iCurKeyframeIndex].fTrackPosition);

		XMVECTOR vLeftScale = {};
		XMVECTOR vRightScale = {};

		vLeftScale = XMLoadFloat3(&m_Keyframes[m_iCurKeyframeIndex].vScale);
		vRightScale = XMLoadFloat3(&m_Keyframes[m_iCurKeyframeIndex + 1].vScale);

		XMVECTOR vLeftRotation = {};
		XMVECTOR vRightRotation = {};

		vLeftRotation = XMLoadFloat4(&m_Keyframes[m_iCurKeyframeIndex].vRotation);
		vRightRotation = XMLoadFloat4(&m_Keyframes[m_iCurKeyframeIndex + 1].vRotation);

		XMVECTOR vLeftTranslate = {};
		XMVECTOR vRightTranslate = {};

		vLeftTranslate = XMVectorSetW(XMLoadFloat3(&m_Keyframes[m_iCurKeyframeIndex].vTranslation), 1.f);
		vRightTranslate = XMVectorSetW(XMLoadFloat3(&m_Keyframes[m_iCurKeyframeIndex + 1].vTranslation), 1.f);

		vScale = XMVectorLerp(vLeftScale, vRightScale, fRatio);
		vRotation = XMQuaternionSlerp(vLeftRotation, vRightRotation, fRatio);
		vTranslation = XMVectorLerp(vLeftTranslate, vRightTranslate, fRatio);

	}
	XMStoreFloat3(&ChannelPos.vCurScale, vScale);
	XMStoreFloat4(&ChannelPos.vCurRotation, vRotation);
	XMStoreFloat3(&ChannelPos.vCurTranslation, vTranslation);
	ChannelPos.iBoneIndex = m_iBoneIndex;
}

CSEChannel* CSEChannel::Create(SE_CHANNEL& seChannel)
{
	CSEChannel* pInstance = new CSEChannel;
	if (FAILED(pInstance->Initialize(seChannel)))
	{
		MSG_BOX("ErrCode : CSEChannel Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CSEChannel* CSEChannel::Clone()
{
	return new CSEChannel(*this);
}

void CSEChannel::Free()
{
	CBase::Free();
}
