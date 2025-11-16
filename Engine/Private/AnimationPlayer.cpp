#include "AnimationPlayer.h"
#include "GameInstance.h"
#include "SEAnimation.h"
#include "SEBone.h"

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
	Update_TransformationMatrix(fDT);
}

void CAnimationPlayer::Change_Animation(const _string& strInitAnimTag)
{
	m_pPreAnimation = m_pCurAnimation;
	m_pCurAnimation = m_pGameInstance->AnimationManager_Get_Animation(strInitAnimTag);
	m_isChangedAnimation = TRUE;
}

void CAnimationPlayer::Update_TransformationMatrix(_float fDT)
{
	m_pCurAnimation->Update_TransformationMatrix(fDT, m_isChangedAnimation);

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
