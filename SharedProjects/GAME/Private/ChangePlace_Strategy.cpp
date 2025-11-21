#include "ChangePlace_Strategy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CChangePlace_Strategy::CChangePlace_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CChangePlace_Strategy::Initialize(PLACE_ID eNextPlaceID)
{
	m_eNextPlaceID = eNextPlaceID;
	return S_OK;
}

void CChangePlace_Strategy::OnClick()
{
	m_pGameInstance->PlaceManager_Change_Place(
		static_cast<uint32_t>(PLACE_ID::LOADING),
		CPlace_Loading::Create(m_pDevice,m_pContext,m_eNextPlaceID));
}

CChangePlace_Strategy* CChangePlace_Strategy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,PLACE_ID eNextPlaceID)
{
	CChangePlace_Strategy* pInstance = new CChangePlace_Strategy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(eNextPlaceID)))
	{
		MSG_BOX("ErrCode : CChangePlace_Strategy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CChangePlace_Strategy::Free()
{
	IOnClick::Free();
}
