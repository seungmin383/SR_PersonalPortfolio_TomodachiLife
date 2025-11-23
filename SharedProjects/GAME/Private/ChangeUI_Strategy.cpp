#include "ChangeUI_Strategy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CChangeUI_Strategy::CChangeUI_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CChangeUI_Strategy::Initialize(PLACEID eCurPlaceID,
	const vector<_wstring>& DisableLayers,
	const vector<_wstring>& EnableLayers)
{
	m_eCurPlaceID = eCurPlaceID;
	m_DisableLayers = DisableLayers;
	m_EnableLayers = EnableLayers;
	return S_OK;
}

void CChangeUI_Strategy::OnClick()
{
	for(auto& DisTag : m_DisableLayers)
		m_pGameInstance->ObjectManager_Disable_Layer(static_cast<uint32_t>(m_eCurPlaceID), DisTag);

	for(auto& EnTag : m_EnableLayers)
		m_pGameInstance->ObjectManager_Enable_Layer(static_cast<uint32_t>(m_eCurPlaceID), EnTag);
}

CChangeUI_Strategy* CChangeUI_Strategy::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	PLACEID eCurPlaceID,
	const vector<_wstring>& DisableLayers,
	const vector<_wstring>& EnableLayers)
{
	CChangeUI_Strategy* pInstance = new CChangeUI_Strategy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(eCurPlaceID, DisableLayers, EnableLayers)))
	{
		MSG_BOX("ErrCode : CChangeUI_Strategy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CChangeUI_Strategy::Free()
{
	IOnClick::Free();
}
