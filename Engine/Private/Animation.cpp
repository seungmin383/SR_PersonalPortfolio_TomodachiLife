#include "Animation.h"
#include "Channel.h"
#include "Model.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(
	SE_ANIMATION& seAnim,
	const aiAnimation* pAIAnimation,
	CModel* pModel)
{
	m_iNumChannels = pAIAnimation->mNumChannels;

	strcpy_s(m_szName, pAIAnimation->mName.data);
	seAnim.strName = pAIAnimation->mName.data;

	auto pos = seAnim.strName.find('|');
	if (pos != std::string::npos)
	{
		seAnim.strName = seAnim.strName.substr(pos + 1); // '|' 뒤부터 끝까지
	}

	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
	seAnim.fTickPerSec = pAIAnimation->mTicksPerSecond;

	m_fDuration = pAIAnimation->mDuration;
	seAnim.fDuration = pAIAnimation->mDuration;

	for (uint32_t i = 0; i < m_iNumChannels; i++)
	{
		SE_CHANNEL seChannel = {};

		CChannel* pChannel = CChannel::Create(
			seChannel,
			pAIAnimation->mChannels[i],
			pModel);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
		seAnim.Channels.push_back(seChannel);
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
	SE_ANIMATION& seAnim,
	const aiAnimation* pAIAnimation,
	CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(seAnim, pAIAnimation, pModel)))
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
