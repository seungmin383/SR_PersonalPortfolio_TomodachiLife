#include "Channel.h"
#include "Bone.h"
#include "Model.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(
	const aiNodeAnim* pAIChannel,
	CModel* pModel)
{
	m_iBoneIndex = pModel->Get_BoneIndex(pAIChannel->mNodeName.data);
	if (-1 == m_iBoneIndex)
		return E_FAIL;

	pAIChannel->mNumPositionKeys;
	pAIChannel->mNumRotationKeys;
	pAIChannel->mNumScalingKeys;

	/* Assimp에서는 변하지 않는 구간을 비워두지 않기 때문에 가능*/
	m_iNumKeyFrames = max(
		pAIChannel->mNumScalingKeys,
		pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(
		m_iNumKeyFrames,
		pAIChannel->mNumPositionKeys);

	_float3 vScale = {};
	_float4 vRotation = {};
	_float3 vTranslation = {};

	for (uint32_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME KeyFrame = {};

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, 
				&pAIChannel->mScalingKeys[i].mValue, 
				sizeof(_float3));

			KeyFrame.fTrackPosition =
				pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;

			KeyFrame.fTrackPosition =
				pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, 
				&pAIChannel->mPositionKeys[i].mValue, 
				sizeof(_float3));

			KeyFrame.fTrackPosition = 
				pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vTranslation = vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}


	return S_OK;
}

void CChannel::Update_TransformationMatrix(
	const vector<CBone*>& Bones,
	_float fCurTrackPosition)
{
	/* 뼈들을 순회하면서 행렬을 만들어줘야됨 
	현재 재생 위치가 마지막 키프레임보다 길어지면
	선형보간이 필요 없음*/

	/* fcurrentTrackPosition >= LastKeyFrame.fTrackPosition */

	if (0 == fCurTrackPosition)
	{
		m_iCurKeyframeIndex = 0;
	}

	XMFLOAT4X4 TransformationMatrix = {};
	KEYFRAME LastKeyFrame = m_KeyFrames.back();

	XMVECTOR vScale = {};
	XMVECTOR vRotation = {};
	XMVECTOR vTranslation = {};

	/* 마지막 키프레임을 넘어갔으면 보간을 안해도 됨 */
	if (fCurTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}
	else
	{
		while (fCurTrackPosition > m_KeyFrames[m_iCurKeyframeIndex + 1].fTrackPosition)
			++m_iCurKeyframeIndex;

		_float fRatio =
			(fCurTrackPosition - m_KeyFrames[m_iCurKeyframeIndex].fTrackPosition) /
			(m_KeyFrames[m_iCurKeyframeIndex + 1].fTrackPosition - m_KeyFrames[m_iCurKeyframeIndex].fTrackPosition);
		
		XMVECTOR vLeftScale = {};
		XMVECTOR vRightScale = {};

		vLeftScale = XMLoadFloat3(&m_KeyFrames[m_iCurKeyframeIndex].vScale);
		vRightScale = XMLoadFloat3(&m_KeyFrames[m_iCurKeyframeIndex + 1].vScale);
		
		XMVECTOR vLeftRotation = {};
		XMVECTOR vRightRotation = {};

		vLeftRotation = XMLoadFloat4(&m_KeyFrames[m_iCurKeyframeIndex].vRotation);
		vRightRotation = XMLoadFloat4(&m_KeyFrames[m_iCurKeyframeIndex + 1].vRotation);

		XMVECTOR vLeftTranslate = {};
		XMVECTOR vRightTranslate = {};

		vLeftTranslate = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[m_iCurKeyframeIndex].vTranslation), 1.f);
		vRightTranslate = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[m_iCurKeyframeIndex + 1].vTranslation),1.f);

		vScale = XMVectorLerp(vLeftScale, vRightScale, fRatio);
		vRotation = XMQuaternionSlerp(vLeftRotation, vRightRotation, fRatio);
		vTranslation = XMVectorLerp(vLeftTranslate, vRightTranslate, fRatio);

	}

	XMStoreFloat4x4(&TransformationMatrix,
		XMMatrixAffineTransformation(
			vScale, 
			XMVectorSet(0.f, 0.f, 0.f, 1.f), 
			vRotation, 
			vTranslation));

	Bones[m_iBoneIndex]->Set_TransformMatrix(TransformationMatrix);
}

CChannel* CChannel::Create(
	const aiNodeAnim* pAIChannel,
	CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("ErrCode : Channel Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	CBase::Free();
}
