#include "AnimationPlayer.h"
#include "GameInstance.h"
#include "SEAnimation.h"
#include "SEBone.h"
#include "SEChannel.h"

CAnimationPlayer::CAnimationPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CAnimationPlayer::CAnimationPlayer(const CAnimationPlayer& rhs)
	:CComponent(rhs)
{
}

HRESULT CAnimationPlayer::Initialize(
	const _string& strInitAnimTag,
	vector<CSEBone*>& pBones)
{
	m_pBones = pBones;
	for (auto& pBone : m_pBones)
		Safe_AddRef(pBone);

	m_strCurAnimTag = strInitAnimTag;
	m_pCurAnimation = 
		m_pGameInstance->AnimationManager_Get_Animation(strInitAnimTag);
	Safe_AddRef(m_pCurAnimation);

	if (nullptr == m_pCurAnimation)
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimationPlayer::Initialize_Clone(void* pArg)
{
	return S_OK;
}

void CAnimationPlayer::Play_Animation(_float fDT)
{
	if (m_isChangedAnimation)
	{
		Blend_Animation(fDT);
	}
	else if(!m_isChangedAnimation)
	{
		Update_TransformationMatrix(fDT);
	}
}

void CAnimationPlayer::Change_Animation(
	const _string& strAnimaTag,
	_float fBlendDuration,
	_bool isLoop)
{
	if (m_strCurAnimTag == strAnimaTag)
		return;
	
	if (m_pPreAnimation != nullptr)
		Safe_Release(m_pPreAnimation);


	m_fBlendDuration = fBlendDuration;
	m_strCurAnimTag = strAnimaTag;
	m_pPreAnimation = m_pCurAnimation;
	m_pCurAnimation = m_pGameInstance->AnimationManager_Get_Animation(strAnimaTag);
	Safe_AddRef(m_pCurAnimation);

	m_isChangedAnimation = TRUE;
	m_isLoop = isLoop;
}

void CAnimationPlayer::Update_TransformationMatrix(_float fDT)
{
	m_pCurAnimation->Update_TransformationMatrix(fDT, m_isChangedAnimation, m_isLoop);
	if (nullptr != m_pPreAnimation)
	{
		m_pPreAnimation->Update_TransformationMatrix(fDT, m_isChangedAnimation, m_isLoop);
	}

	uint32_t NumChannel = m_pCurAnimation->Get_NumChannel();
	const vector<CHANNEL_POS>& ChannelPos = m_pCurAnimation->Get_ChannelPos();

	for (uint32_t i = 0; i < NumChannel; ++i)
	{
		XMFLOAT4X4 TransformationMatrix = {};

		XMStoreFloat4x4(&TransformationMatrix,
			XMMatrixAffineTransformation(
				XMLoadFloat3(& ChannelPos[i].vCurScale),
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMLoadFloat4(&ChannelPos[i].vCurRotation),
				XMVectorSetW(XMLoadFloat3(&ChannelPos[i].vCurTranslation), 1.f)));

		//여기서 채널의 BoneInddex를 가져와야됨
		m_pBones[ChannelPos[i].iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
	}
}

void CAnimationPlayer::Blend_Animation(_float fDT)
{
	_bool isFalse = FALSE;
	m_pCurAnimation->Update_TransformationMatrix(fDT, isFalse, m_isLoop);
	m_pPreAnimation->Update_TransformationMatrix(fDT, isFalse, m_isLoop);

	uint32_t NumChannel = m_pCurAnimation->Get_NumChannel();
	const vector<CHANNEL_POS>& CurChannelPos = m_pCurAnimation->Get_ChannelPos();
	const vector<CHANNEL_POS>& PreChannelPos = m_pPreAnimation->Get_ChannelPos();
	const vector<CSEChannel*> tmpChannel = m_pPreAnimation->Get_Channel();

	m_fBlendTime += fDT;
	_float fRatio = 0.f;
	if (m_fBlendDuration > 0.f)
		fRatio = m_fBlendTime / m_fBlendDuration;

	if (m_fBlendDuration < m_fBlendTime)
	{
		m_fBlendTime = 0.f;
		m_pCurAnimation->Update_TransformationMatrix(fDT, m_isChangedAnimation, m_isLoop);
		m_pPreAnimation->Update_TransformationMatrix(fDT, m_isChangedAnimation, m_isLoop);

		m_isChangedAnimation = FALSE;
		return;
	}

	// 0 ~ 1로 클램프
	if (fRatio < 0.f) fRatio = 0.f;
	if (fRatio > 1.f) fRatio = 1.f;

	for (uint32_t i = 0; i < NumChannel; ++i)
	{
		const CHANNEL_POS& pre = PreChannelPos[i];
		const CHANNEL_POS& cur = CurChannelPos[i];

		// 3-1) S / R / T 로드
		XMVECTOR vPreScale = XMLoadFloat3(&pre.vCurScale);
		XMVECTOR vCurScale = XMLoadFloat3(&cur.vCurScale);

		XMVECTOR vPreRotation = XMLoadFloat4(&pre.vCurRotation);
		XMVECTOR vCurRotation = XMLoadFloat4(&cur.vCurRotation);

		XMVECTOR vPreTranslation = XMVectorSetW(XMLoadFloat3(&pre.vCurTranslation), 1.f);
		XMVECTOR vCurTranslation = XMVectorSetW(XMLoadFloat3(&cur.vCurTranslation), 1.f);

		// 3-2) LERP / SLERP
		XMVECTOR vScale = XMVectorLerp(vPreScale, vCurScale, fRatio);
		XMVECTOR vRotation = XMQuaternionSlerp(vPreRotation, vCurRotation, fRatio);
		XMVECTOR vTranslation = XMVectorLerp(vPreTranslation, vCurTranslation, fRatio);

		// 3-3) 최종 본 행렬 생성
		XMFLOAT4X4 TransformationMatrix = {};
		XMStoreFloat4x4(
			&TransformationMatrix,
			XMMatrixAffineTransformation(
				vScale,
				XMVectorSet(0.f, 0.f, 0.f, 1.f), // 회전 중심(원점)
				vRotation,
				vTranslation));

		// 3-4) 본 인덱스에 적용
		uint32_t iBoneIndex = static_cast<uint32_t>(cur.iBoneIndex); // 또는 pre.iBoneIndex
		if (iBoneIndex < m_pBones.size() && m_pBones[iBoneIndex])
		{
			m_pBones[iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
		}
	}
}

CAnimationPlayer* CAnimationPlayer::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	const _string& strInitAnimationTag,
	vector<CSEBone*>& pBones)
{
	CAnimationPlayer* pInstance = new CAnimationPlayer(pDevice, pContext);
	if (FAILED(pInstance->Initialize(strInitAnimationTag, pBones)))
	{
		MSG_BOX("ErrCode : CAnimationPlayer Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CAnimationPlayer::Clone(void* pArg)
{
	CAnimationPlayer* pInstance = new CAnimationPlayer(*this);
	if(FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CAnimationPlayer Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAnimationPlayer::Free()
{
	for (auto& pBone : m_pBones)
		Safe_Release(pBone);
	m_pBones.clear();
	Safe_Release(m_pCurAnimation);
	Safe_Release(m_pPreAnimation);

	CComponent::Free();
}
