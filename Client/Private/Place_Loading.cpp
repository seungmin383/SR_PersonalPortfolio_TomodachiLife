#include "Place_Loading.h"
#include "Loader.h"
#include "GameInstance.h"


CPlace_Loading::CPlace_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CPlace(_pDevice, _pContext)
{
}

HRESULT CPlace_Loading::Initialize(PLACE_ID _eNextLevelID)
{
	m_eNextLevelID = _eNextLevelID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;


	return S_OK;
}

int32_t CPlace_Loading::Update(const _float fDT)
{
	if (true == m_pLoader->isFinished() &&
		GetKeyState(VK_RETURN) & 0x8000)
	{
		CPlace* pNewPlace = { nullptr };

		switch (m_eNextLevelID)
		{
		case Client::PLACE_ID::LOADING:
			break;
		case Client::PLACE_ID::SETTING:
			break;
		case Client::PLACE_ID::SAVE:
			break;
		case Client::PLACE_ID::LOAD:
			break;
		case Client::PLACE_ID::MII_EDITOR:
			break;
		case Client::PLACE_ID::STORE_CLOTHES:
			break;
		case Client::PLACE_ID::STORE_ANTIQUE:
			break;
		case Client::PLACE_ID::STORE_HAT:
			break;
		case Client::PLACE_ID::STORE_FOOD:
			break;
		case Client::PLACE_ID::STORE_LUXURY:
			break;
		case Client::PLACE_ID::STORE_HOUSE:
			break;
		case Client::PLACE_ID::ENVI_FOUNTAIN:
			break;
		case Client::PLACE_ID::ENVI_OCEAN:
			break;
		case Client::PLACE_ID::ENVI_PARK:
			break;
		case Client::PLACE_ID::ENVI_PLAYGROUND:
			break;
		case Client::PLACE_ID::ENVI_CAFE:
			break;
		case Client::PLACE_ID::ETC_COMMUINTY:
			break;
		case Client::PLACE_ID::ETC_MUSIC:
			break;
		case Client::PLACE_ID::ETC_RANKING:
			break;
		case Client::PLACE_ID::ETC_NEWS:
			break;
		case Client::PLACE_ID::ETC_PHOTO:
			break;
		case Client::PLACE_ID::CONNECT_TENT:
			break;
		case Client::PLACE_ID::CONNECT_PORT:
			break;
		case Client::PLACE_ID::LIVE_APARTMENT:
			break;
		case Client::PLACE_ID::LIVE_WEDDING_HOUSE:
			break;
		case Client::PLACE_ID::END:
			break;
		default:
			break;
		}

		if (nullptr == pNewPlace)
			return 1;

		if (SUCCEEDED(m_pGameInstance->PlaceManager_Go_Place(pNewPlace)))
			return 0;

		MSG_BOX(" ErrCode : ¾î? ");
	}
}

HRESULT CPlace_Loading::Render()
{
	return S_OK;
}

HRESULT CPlace_Loading::Ready_Layer_UI()
{
	return S_OK;
}

CPlace_Loading* CPlace_Loading::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, PLACE_ID _eNextLevelID)
{
	CPlace_Loading* pPlaceLoading = new CPlace_Loading(_pDevice, _pContext);

	if (FAILED(pPlaceLoading->Initialize(_eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pPlaceLoading);
	}
	return pPlaceLoading;
}

void CPlace_Loading::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CPlace::Free();
}
