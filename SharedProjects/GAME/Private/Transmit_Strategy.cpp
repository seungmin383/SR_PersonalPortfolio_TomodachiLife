#include "Transmit_Strategy.h"
#include "MiiManager.h"

CTransmit_Strategy::CTransmit_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext),
	m_pMiiManager(CMiiManager::GetInstance())
{
	Safe_AddRef(m_pMiiManager);
}

HRESULT CTransmit_Strategy::Initialize(PARTS eParts, uint32_t iIndex)
{
	m_eParts = eParts;
	m_iIndex = iIndex;

	return S_OK;
}

void CTransmit_Strategy::OnClick()
{
	switch (m_eParts)
	{
	case Game::CTransmit_Strategy::HEAD:
		m_pMiiManager->Get_HeadInfo().iIndex = m_iIndex;
		break;
	case Game::CTransmit_Strategy::HAIR:
		m_pMiiManager->Get_HairInfo().iIndex = m_iIndex;
		break;
	case Game::CTransmit_Strategy::EYEBROW:
		m_pMiiManager->Get_EyebrowInfo().iIndex = m_iIndex;
		break;
	case Game::CTransmit_Strategy::EYES:
		m_pMiiManager->Get_EyesInfo().iIndex = m_iIndex;
		break;
	case Game::CTransmit_Strategy::NOSE:
		m_pMiiManager->Get_NoseInfo().iIndex = m_iIndex;
		break;
	case Game::CTransmit_Strategy::MOUTH:
		m_pMiiManager->Get_MouthInfo().iIndex = m_iIndex;
		break;
	case Game::CTransmit_Strategy::END:
		break;
	default:
		break;
	}
}

CTransmit_Strategy* CTransmit_Strategy::Create(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PARTS eParts, uint32_t iIndex)
{
	CTransmit_Strategy* pInstance = new CTransmit_Strategy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(eParts, iIndex)))
	{
		MSG_BOX("ErrCode : CTransmit_Strategy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTransmit_Strategy::Free()
{
	Safe_Release(m_pMiiManager);
	IOnClick::Free();
}

