#include "Loader.h"
#include "GameInstance.h"
#include "Food.h"

USING(Engine)

CLoader::CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pGameInstance);
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

	m_hThread = (HANDLE)_beginthreadex(
		nullptr, 0, ThreadMain, this, 0, nullptr);
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
	case Client::PLACE_ID::MAIN:
		hr = Loading_For_Main();
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

	CoUninitialize();
	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}


HRESULT CLoader::Loading_For_Main()
{
	CObject* tmp = { nullptr };

#pragma region ACTOR_LOADING

	lstrcpy(m_szFPS, TEXT("Actor Loading Complete"));
#pragma endregion


#pragma region UI_LOADING

	tmp = CFood::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(OBJECT_TYPE::UI),
		static_cast<uint32_t>(OBJECT_ID::UI_PROTO_FOOD),
		tmp)))
	{
		MSG_BOX("ErrCode : Food Prototype Add Failed");
		return E_FAIL;
	}

	lstrcpy(m_szFPS, TEXT("UI Loading Complete"));
#pragma endregion


#pragma region COMPONENT_LOADING

	tmp = CShaderCom::Create(
		m_pDevice,
		m_pContext,
		L"../Bin/Debug/ShaderFile/Shader_Vtx_Tex.hlsl",
		VTX_TEX::Elements,
		VTX_TEX::iNumElements);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(OBJECT_TYPE::COMPONENT),
		static_cast<uint32_t>(OBJECT_ID::COMP_PROTO_SHADER),
		tmp)))
	{
		MSG_BOX("ErrCode : Shader Prototype Add Failed");
		return E_FAIL;
	}


	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../Bin/Debug/Resource/Textures/Default%d.jpg"),
		2); 

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(OBJECT_TYPE::COMPONENT),
		static_cast<uint32_t>(OBJECT_ID::COMP_PROTO_TEXTURE),
		tmp)))
	{
		MSG_BOX("ErrCode : Texture Prototype Add Failed");
		return E_FAIL;
	}

	tmp = CRectBufferCom::Create(m_pDevice,m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(OBJECT_TYPE::COMPONENT),
		static_cast<uint32_t>(OBJECT_ID::COMP_PROTO_RECT_BUFFER),
		tmp)))
	{
		MSG_BOX("ErrCode : Rect Buffer Prototype Add Failed");
		return E_FAIL;
	}

	lstrcpy(m_szFPS, TEXT("Component Loading Complete"));
#pragma endregion


	lstrcpy(m_szFPS, TEXT("Main Place Loading Complete"));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Setting()
{
	lstrcpy(m_szFPS, TEXT("Loading_For_Setting 완료되었슴니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Save()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_MiiEditor()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Clothes()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Antique()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Hat()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Food()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Store_Luxury()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Store_House()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Fountain()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Ocean()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Park()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Playground()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Envi_Cafe()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Community()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Music()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Ranking()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_News()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Etc_Photo()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Connect_Tent()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Connect_Port()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Live_Apartment()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	return S_OK;
}

HRESULT CLoader::Loading_For_Live_Wedding_House()
{
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

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
	
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CBase::Free();
}
