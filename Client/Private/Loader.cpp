#include "Loader.h"

CLoader::CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY ThreadMain(void* _pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(_pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(PLACE_ID _eNextPlaceID)
{
	m_eNextPlaceID = _eNextPlaceID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

#pragma region LOADING_FUCTION

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT		hr = {};

	switch (m_eNextPlaceID)
	{
	case Client::PLACE_ID::LOADING:
		hr = Loading_For_Setting();
		break;
	case Client::PLACE_ID::SETTING:
		hr = Loading_For_Setting();
		break;
	case Client::PLACE_ID::SAVE:
		hr = Loading_For_Save();
		break;
	case Client::PLACE_ID::MII_EDITOR:
		hr = Loading_For_MiiEditor();
		break;
	case Client::PLACE_ID::STORE_CLOTHES:
		hr = Loading_For_Store_Clothes();
		break;
	case Client::PLACE_ID::STORE_ANTIQUE:
		hr = Loading_For_Store_Antique();
		break;
	case Client::PLACE_ID::STORE_HAT:
		hr = Loading_For_Store_Hat();
		break;
	case Client::PLACE_ID::STORE_FOOD:
		hr = Loading_For_Store_Food();
		break;
	case Client::PLACE_ID::STORE_LUXURY:
		hr = Loading_For_Store_Luxury();
		break;
	case Client::PLACE_ID::STORE_HOUSE:
		hr = Loading_For_Store_House();
		break;
	case Client::PLACE_ID::ENVI_FOUNTAIN:
		hr = Loading_For_Envi_Fountain();
		break;
	case Client::PLACE_ID::ENVI_OCEAN:
		hr = Loading_For_Envi_Ocean();
		break;
	case Client::PLACE_ID::ENVI_PARK:
		hr = Loading_For_Envi_Park();
		break;
	case Client::PLACE_ID::ENVI_PLAYGROUND:
		hr = Loading_For_Envi_Playground();
		break;
	case Client::PLACE_ID::ENVI_CAFE:
		hr = Loading_For_Envi_Cafe();
		break;
	case Client::PLACE_ID::ETC_COMMUINTY:
		hr = Loading_For_Etc_Community();
		break;
	case Client::PLACE_ID::ETC_MUSIC:
		hr = Loading_For_Etc_Music();
		break;
	case Client::PLACE_ID::ETC_RANKING:
		hr = Loading_For_Etc_Ranking();
		break;
	case Client::PLACE_ID::ETC_NEWS:
		hr = Loading_For_Etc_News();
		break;
	case Client::PLACE_ID::ETC_PHOTO:
		hr = Loading_For_Etc_Photo();
		break;
	case Client::PLACE_ID::CONNECT_TENT:
		hr = Loading_For_Connect_Tent();
		break;
	case Client::PLACE_ID::CONNECT_PORT:
		hr = Loading_For_Connect_Port();
		break;
	case Client::PLACE_ID::LIVE_APARTMENT:
		hr = Loading_For_Live_Apartment();
		break;
	case Client::PLACE_ID::LIVE_WEDDING_HOUSE:
		hr = Loading_For_Live_Wedding_House();
		break;
	case Client::PLACE_ID::END:
		break;
	default:
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}


HRESULT CLoader::Loading_For_Setting()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Save()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_MiiEditor()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Clothes()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Antique()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Hat()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Food()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Luxury()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Store_House()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Fountain()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Ocean()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Park()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Playground()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Cafe()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Community()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Music()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Ranking()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_News()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Photo()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Connect_Tent()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Connect_Port()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Live_Apartment()
{
	return S_OK;
}

HRESULT CLoader::Loading_For_Live_Wedding_House()
{
	return S_OK;
}
#pragma endregion

CLoader* CLoader::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, PLACE_ID _eNextPlaceID)
{
	CLoader* pLoader = new CLoader(_pDevice, _pContext);

	if (FAILED(pLoader->Initialize(_eNextPlaceID)))
	{
		MSG_BOX("ErrCode : Loader Create Failed");
		Safe_Release(pLoader);
	}
	return pLoader;
}

void CLoader::Free()
{
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}

	DeleteCriticalSection(&m_CriticalSection);
	
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CBase::Free();
}
