#include "ButtonManager.h"
#include "Button.h"
#include "GameInstance.h"

CButtonManager::CButtonManager(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);

}

HRESULT CButtonManager::Initialize(uint32_t iNumPlace)
{
	m_iNumPlace = iNumPlace;

	m_Buttons = new vector<CButton*>[iNumPlace];
	return S_OK;
}

HRESULT CButtonManager::Add_Button(uint32_t iPlaceID, CButton* pButton)
{
	Safe_AddRef(pButton);
	m_Buttons[iPlaceID].push_back(pButton);
	return S_OK;
}

void CButtonManager::All_Deactivate(uint32_t iPlaceID)
{
	for (auto& pButton : m_Buttons[iPlaceID])
	{
		pButton->Set_isSelected(FALSE);
	}
}

CButtonManager* CButtonManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t iNumPlace)
{
	CButtonManager* pInstance = new CButtonManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumPlace)))
	{
		MSG_BOX("ErrCode : ButtonManager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CButtonManager::Free()
{
	CBase::Free();

	for (uint32_t i = 0; i < m_iNumPlace; i++)
	{
		for (auto& pButton : m_Buttons[i])
		{
			Safe_Release(pButton);
		}
		m_Buttons[i].clear();
	}
	Safe_Delete_Array(m_Buttons);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
