#include "ChangePlace_Strategy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CChangePlace_Strgy::CChangePlace_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CChangePlace_Strgy::Initialize(PLACEID eNextPlaceID, const _wstring& strLayerTag)
{
	m_eNextPlaceID = eNextPlaceID;
	m_strLayerTag = strLayerTag;

	return S_OK;
}

void CChangePlace_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(PLACEID::ISLAND), m_strLayerTag);
	m_pGameInstance->PlaceManager_Change_Place(	static_cast<uint32_t>(PLACEID::LOADING), CPlace_Loading::Create(m_pDevice,m_pContext,m_eNextPlaceID));
}

CChangePlace_Strgy* CChangePlace_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,PLACEID eNextPlaceID, const _wstring& strLayerTag )
{
	CChangePlace_Strgy* pInstance = new CChangePlace_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(eNextPlaceID, strLayerTag)))
	{
		MSG_BOX("ErrCode : CChangePlace_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CChangePlace_Strgy::Free()
{
	IOnClick::Free();
}
