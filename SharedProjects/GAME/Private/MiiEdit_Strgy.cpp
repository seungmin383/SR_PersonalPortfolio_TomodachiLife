#include "MiiEdit_Strgy.h"	
#include "GameInstance.h"
#include "MiiManager.h"

CMiiEdit_Strgy::CMiiEdit_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CMiiEdit_Strgy::Initialize(_wstring strLayerTag, PARTS ePart, uint32_t iIndex)
{
	m_strLayerTag = strLayerTag;
	m_ePart = ePart;
	m_iIndex = iIndex;
	return S_OK;
}

void CMiiEdit_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(PLACEID::MII_EDITOR), m_strLayerTag);

	switch (m_ePart)
	{
	case Game::CMiiEdit_Strgy::HEAD:
		CMiiManager::GetInstance()->Get_Edit_HeadInfo().iIndex = m_iIndex;
		break;
	case Game::CMiiEdit_Strgy::HAIR:
		CMiiManager::GetInstance()->Get_Edit_HairInfo().iIndex = m_iIndex;
		break;
	case Game::CMiiEdit_Strgy::EYEBROW:
		CMiiManager::GetInstance()->Get_Edit_EyebrowInfo().iIndex = m_iIndex;
		break;
	case Game::CMiiEdit_Strgy::EYES:
		CMiiManager::GetInstance()->Get_Edit_EyesInfo().iIndex = m_iIndex;
		break;
	case Game::CMiiEdit_Strgy::NOSE:
		CMiiManager::GetInstance()->Get_Edit_NoseInfo().iIndex = m_iIndex;
		break;
	case Game::CMiiEdit_Strgy::MOUTH:
		CMiiManager::GetInstance()->Get_Edit_MouthInfo().iIndex = m_iIndex;
		break;

	case Game::CMiiEdit_Strgy::END:
		break;
	default:
		break;
	}
}

CMiiEdit_Strgy* CMiiEdit_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _wstring strLayerTag, PARTS ePart, uint32_t iIndex)
{
	CMiiEdit_Strgy* pInstance = new CMiiEdit_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(strLayerTag, ePart, iIndex)))
	{
		MSG_BOX("ErrCode : CMiiEdit_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiEdit_Strgy::Free()
{
	IOnClick::Free();
}
