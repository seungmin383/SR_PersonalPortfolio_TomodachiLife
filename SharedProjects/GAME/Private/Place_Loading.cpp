#include "Place_Loading.h"
#include "Loader.h"
#include "GameInstance.h"

#include "Place_Main.h"
#include "Place_Setting.h"
#include "Place_Island.h"

CPlace_Loading::CPlace_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Loading::Initialize(
	PLACE_ID NextPlaceID)
{
	m_eNextPlaceID = NextPlaceID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextPlaceID);

	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_Loading::Update(_float fDT)
{
	if (m_pLoader->isFinished())
	{
		CPlace* pPlace = { nullptr };

		switch (m_eNextPlaceID)
		{
		case PLACE_ID::STATIC:
			break;

		case PLACE_ID::MAIN:
			pPlace = CPlace_Main::Create(m_pDevice, m_pContext);
			break;

		case PLACE_ID::LOADING:
			break;

		case PLACE_ID::SETTING:
			pPlace = CPlace_Setting::Create(m_pDevice, m_pContext);
			break;

		case PLACE_ID::SAVE:
			break;

		case PLACE_ID::MII_EDITOR:
			break;
		case PLACE_ID::STORE_CLOTHES:
			break;
		case PLACE_ID::STORE_ANTIQUE:
			break;
		case PLACE_ID::STORE_HAT:
			break;
		case PLACE_ID::STORE_FOOD:
			break;
		case PLACE_ID::STORE_LUXURY:
			break;
		case PLACE_ID::STORE_HOUSE:
			break;
		case PLACE_ID::ENVI_FOUNTAIN:
			break;
		case PLACE_ID::ENVI_OCEAN:
			break;
		case PLACE_ID::ENVI_PARK:
			break;
		case PLACE_ID::ENVI_PLAYGROUND:
			break;
		case PLACE_ID::ENVI_CAFE:
			break;
		case PLACE_ID::ETC_COMMUINTY:
			break;
		case PLACE_ID::ETC_MUSIC:
			break;
		case PLACE_ID::ETC_RANKING:
			break;
		case PLACE_ID::ETC_NEWS:
			break;
		case PLACE_ID::ETC_PHOTO:
			break;
		case PLACE_ID::CONNECT_TENT:
			break;
		case PLACE_ID::CONNECT_PORT:
			break;
		case PLACE_ID::LIVE_APARTMENT:
			break;
		case PLACE_ID::LIVE_WEDDING_HOUSE:
			break;
		case PLACE_ID::ISLAND:
			pPlace = CPlace_Island::Create(m_pDevice, m_pContext);
			break;
		case PLACE_ID::END:
			break;
		default:
			break;
		}

		if (nullptr == pPlace)
			return 1;

		if (SUCCEEDED(m_pGameInstance->PlaceManager_Change_Place(
			static_cast<uint32_t>(m_eNextPlaceID),
			pPlace)))
			return 1;

		MSG_BOX(" ErrCode : 왜 여기까지 왔지? ");
	}

	return 0;
}

HRESULT CPlace_Loading::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("로딩레벨입니다."));
	return S_OK;
}

HRESULT CPlace_Loading::Render_Late()
{
	return S_OK;
}

CPlace_Loading* CPlace_Loading::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	PLACE_ID NextPlaceID)
{
	CPlace_Loading* pPlaceLoading = new CPlace_Loading(pDevice, pContext);

	if (FAILED(pPlaceLoading->Initialize(NextPlaceID)))
	{
		MSG_BOX("ErrCode : Loading Place Create Failed");
		Safe_Release(pPlaceLoading);
		return nullptr;
	}

	return pPlaceLoading;
}

void CPlace_Loading::Free()
{
	Safe_Release(m_pLoader);

	CPlace::Free();
}
