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
	:CComponent(rhs),
	m_fCurTrackPosition(rhs.m_fCurTrackPosition),
	m_fBlendDuration(rhs.m_fBlendDuration),
	m_fBlendTime(rhs.m_fBlendTime)
{
}

HRESULT CAnimationPlayer::Initialize()
{
	return S_OK;
}

HRESULT CAnimationPlayer::Initialize_Clone(void* pArg)
{
	ANIMATIONPLAYER_DESC* pDesc = static_cast<ANIMATIONPLAYER_DESC*>(pArg);
	m_pBones = (pDesc->m_pBones);
	m_strCurAnimTag = pDesc->strInitAnimTag;
	m_TargetPreTransformMatrix = pDesc->PreTransformMatrix;

	m_pCurAnimation = m_pGameInstance->AnimationManager_Find_Animation(pDesc->strInitAnimTag);

	return S_OK;
}

void CAnimationPlayer::Play_Animation(_float fDT)
{
	if (nullptr == m_pCurAnimation)
		return;

	if (m_isChangedAnimation)
	{
		Blend_Animation(fDT);
	}
	else if(!m_isChangedAnimation)
	{
		Update_TransformationMatrix(fDT);
	}

	for (auto& pBone : *m_pBones)
	{
		pBone->Update_CombinedTransformMatrix(*m_pBones, XMLoadFloat4x4(&m_TargetPreTransformMatrix));
	}
}

/* Play_Animation이 돌고 있을 때를 가정함 */
_bool CAnimationPlayer::Play_Macro(const _wstring& strMacroTag, _float fDT)
{
	auto iter = m_AnimationMacro.find(strMacroTag);

	if (m_AnimationMacro.end() == iter)
	{
		MSG_BOX("ErrCode : No Macro");
		return TRUE;
	}

	if (m_iCurMacroIndex == 0 && m_pCurAnimation == nullptr)
		m_pCurAnimation = m_pGameInstance->AnimationManager_Find_Animation(iter->second[0]);


	/* 애니메이션이 끝났으면 */
	if (m_isAnimFin)
	{
		m_iCurMacroIndex++;

		if (m_iCurMacroIndex >= iter->second.size())
		{
			m_isAnimFin = FALSE;
			m_iCurMacroIndex = 0;
			return TRUE;
		}

		const _wstring& strAnimTag = iter->second[m_iCurMacroIndex];
		Change_Animation(strAnimTag, 0.2f, FALSE);
		m_isAnimFin = FALSE;
	}

	return FALSE;
}

void CAnimationPlayer::Change_Animation(const _wstring& strAnimaTag, _float fBlendDuration, _bool isLoop)
{
	if (m_strCurAnimTag == strAnimaTag)
		return;
	
	if (m_pPreAnimation != nullptr)
		Safe_Release(m_pPreAnimation);

	m_fBlendDuration = fBlendDuration;
	m_strCurAnimTag = strAnimaTag;
	
	m_pPreAnimation = m_pCurAnimation;
	m_fPreTrackPosition = m_fCurTrackPosition;
	m_fCurTrackPosition = 0.f;

	m_pCurAnimation = m_pGameInstance->AnimationManager_Find_Animation(strAnimaTag);

	m_isChangedAnimation = TRUE;
	m_isLoop = isLoop;
}

HRESULT CAnimationPlayer::Add_Animation_Macro(const _wstring& strMacroTag, vector<_wstring> AnimTags)
{
	auto iter = m_AnimationMacro.find(strMacroTag);
	if (m_AnimationMacro.end() != iter)
		return E_FAIL;

	m_AnimationMacro.emplace(strMacroTag, AnimTags);

	return S_OK;
}

void CAnimationPlayer::Update_TransformationMatrix(_float fDT)
{
	m_isAnimFin = m_pCurAnimation->Update_TransformationMatrix(m_fCurTrackPosition, fDT, m_isLoop);

	if (m_isAnimFin == TRUE)
		int a = 0;

	if (nullptr != m_pPreAnimation)
	{
		m_pPreAnimation->Update_TransformationMatrix(m_fPreTrackPosition, fDT, m_isLoop);
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
		(*m_pBones)[ChannelPos[i].iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
	}
}

void CAnimationPlayer::Blend_Animation(_float fDT)
{
	_bool isFalse = FALSE;
	m_pCurAnimation->Update_TransformationMatrix(m_fCurTrackPosition, fDT, m_isLoop);
	m_pPreAnimation->Update_TransformationMatrix(m_fPreTrackPosition, fDT, m_isLoop);

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
		m_pCurAnimation->Update_TransformationMatrix(m_fCurTrackPosition, fDT, m_isLoop);
		m_pPreAnimation->Update_TransformationMatrix(m_fPreTrackPosition, fDT, m_isLoop);

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

		XMVECTOR vPreScale = XMLoadFloat3(&pre.vCurScale);
		XMVECTOR vCurScale = XMLoadFloat3(&cur.vCurScale);

		XMVECTOR vPreRotation = XMLoadFloat4(&pre.vCurRotation);
		XMVECTOR vCurRotation = XMLoadFloat4(&cur.vCurRotation);

		XMVECTOR vPreTranslation = XMVectorSetW(XMLoadFloat3(&pre.vCurTranslation), 1.f);
		XMVECTOR vCurTranslation = XMVectorSetW(XMLoadFloat3(&cur.vCurTranslation), 1.f);

		XMVECTOR vScale = XMVectorLerp(vPreScale, vCurScale, fRatio);
		XMVECTOR vRotation = XMQuaternionSlerp(vPreRotation, vCurRotation, fRatio);
		XMVECTOR vTranslation = XMVectorLerp(vPreTranslation, vCurTranslation, fRatio);

		XMFLOAT4X4 TransformationMatrix = {};
		XMStoreFloat4x4(&TransformationMatrix, XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));

		uint32_t iBoneIndex = static_cast<uint32_t>(cur.iBoneIndex); // 또는 pre.iBoneIndex
		if (iBoneIndex < (*m_pBones).size() && (*m_pBones)[iBoneIndex])
		{
			(*m_pBones)[iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
		}
	}
}

CAnimationPlayer* CAnimationPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAnimationPlayer* pInstance = new CAnimationPlayer(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
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
	Safe_Release(m_pCurAnimation);
	Safe_Release(m_pPreAnimation);

	CComponent::Free();
}
