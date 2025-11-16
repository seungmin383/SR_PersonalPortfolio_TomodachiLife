#include "AnimationPlayer.h"
#include "GameInstance.h"
#include "SEAnimation.h"

CAnimationPlayer::CAnimationPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CAnimationPlayer::CAnimationPlayer(const CAnimationPlayer& rhs)
	:CComponent(rhs)
{
}

HRESULT CAnimationPlayer::Initialize(const _string& strInitAnimTag)
{
	m_pCurAnimation = 
		m_pGameInstance->AnimationManager_Get_Animation(strInitAnimTag);
	
	if (nullptr == m_pCurAnimation)
		return E_FAIL;

	return S_OK;
}

void CAnimationPlayer::Play_Animation()
{

}

void CAnimationPlayer::Change_Animation(const _string& strInitAnimTag)
{
}

void CAnimationPlayer::Update_TransformationMatrix()
{
}

CAnimationPlayer* CAnimationPlayer::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	const _string& strInitAnimationTag)
{
	CAnimationPlayer* pInstance = new CAnimationPlayer(pDevice, pContext);
	if (FAILED(pInstance->Initialize(strInitAnimationTag)))
	{
		MSG_BOX("ErrCode : CAnimationPlayer Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CAnimationPlayer::Clone(void* pArg)
{
	return nullptr;
}

void CAnimationPlayer::Free()
{
	Safe_Release(m_pCurAnimation);
	Safe_Release(m_pPreAnimation);
	CComponent::Free();
}
