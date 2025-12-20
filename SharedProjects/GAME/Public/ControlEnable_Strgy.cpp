#include "ControlEnable_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CControlEnable_Strgy::CControlEnable_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CControlEnable_Strgy::Initialize(const CONTROLENABLESTRGY_DESC& Desc)
{
	m_iPlaceID = static_cast<uint32_t>(Desc.ePlaceID);
	m_EnableLayers = Desc.EnableLayers;
	m_DisableLayers = Desc.DisableLayers;
	m_strLayerTag = Desc.strLayerTag;

	return S_OK;
}

void CControlEnable_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_All_Deactivate(m_iPlaceID);

	for (auto& str : m_EnableLayers)
		m_pGameInstance->ObjectManager_Enable_Layer(m_iPlaceID, str);

	for (auto& str : m_DisableLayers)
		m_pGameInstance->ObjectManager_Disable_Layer(m_iPlaceID, str);
}

CControlEnable_Strgy* CControlEnable_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CONTROLENABLESTRGY_DESC& Desc)
{
	CControlEnable_Strgy* pInstance = new CControlEnable_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX("ErrCode : CControlEnable_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CControlEnable_Strgy::Free()
{
	IOnClick::Free();
}
