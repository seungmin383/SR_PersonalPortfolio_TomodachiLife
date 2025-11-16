#include "SEChannel.h"

CSEChannel::CSEChannel()
{
}

HRESULT CSEChannel::Initialize(SE_CHANNEL& seChannel)
{
	m_iBoneIndex = seChannel.iBoneIndex;

	uint32_t NumKeyframe = seChannel.Keyframes.size();

	for (uint32_t i = 0; i < NumKeyframe; ++i)
	{
		KEYFRAME kf = seChannel.Keyframes[i];
		m_Keyframes.push_back(kf);
	}
	return S_OK;
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

void CSEChannel::Free()
{
	CBase::Free();
}
