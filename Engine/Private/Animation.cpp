#include "Animation.h"
#include "Channel.h"
#include "Model.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(
	const aiAnimation* pAIAnimation,
	CModel* pModel)
{
	m_iNumChannels = pAIAnimation->mNumChannels;

	strcpy_s(m_szName, pAIAnimation->mName.data);
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
	m_fDuration = pAIAnimation->mDuration;


	for (uint32_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(
			pAIAnimation->mChannels[i],
			pModel);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrices(
	const vector<CBone*>& Bones, 
	_float fDT, 
	_bool isLoop)
{
	m_fCurTrackPosition += m_fTickPerSecond * fDT;

	if (m_fCurTrackPosition >= m_fDuration)
	{
		if (!isLoop)
			return TRUE;
	
		m_fCurTrackPosition = 0.f;
	}

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(Bones, m_fCurTrackPosition);
	}

	return FALSE;
}


CAnimation* CAnimation::Create(
	const aiAnimation* pAIAnimation,
	CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("ErrCode : Animation Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();

	CBase::Free();
}
