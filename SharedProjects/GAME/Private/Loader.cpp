#include "Loader.h"
#include "GameInstance.h"
#include "Food.h"
#include "Island.h"
#include "Mii.h"
#include "WorldCamera.h"
#include "UICamera.h"
#include "PlaceMoveButton.h"

USING(Engine)

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(pDevice), m_pContext(pContext)
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

HRESULT CLoader::Initialize(PLACE_ID NextPlaceID)
{
	m_eNextPlaceID = NextPlaceID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(
		nullptr, 0, ThreadMain, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

// 
#pragma region LOADING_FUCTION

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT		hr = {};

	switch (m_eNextPlaceID)
	{
	case PLACE_ID::MAIN:
		hr = Loading_For_Main();
		break;
	case PLACE_ID::SETTING:
		hr = Loading_For_Setting();
		break;
	case PLACE_ID::SAVE:
		hr = Loading_For_Save();
		break;
	case PLACE_ID::MII_EDITOR:
		hr = Loading_For_MiiEditor();
		break;
	case PLACE_ID::STORE_CLOTHES:
		hr = Loading_For_Store_Clothes();
		break;
	case PLACE_ID::STORE_ANTIQUE:
		hr = Loading_For_Store_Antique();
		break;
	case PLACE_ID::STORE_HAT:
		hr = Loading_For_Store_Hat();
		break;
	case PLACE_ID::STORE_FOOD:
		hr = Loading_For_Store_Food();
		break;
	case PLACE_ID::STORE_LUXURY:
		hr = Loading_For_Store_Luxury();
		break;
	case PLACE_ID::STORE_HOUSE:
		hr = Loading_For_Store_House();
		break;
	case PLACE_ID::ENVI_FOUNTAIN:
		hr = Loading_For_Envi_Fountain();
		break;
	case PLACE_ID::ENVI_OCEAN:
		hr = Loading_For_Envi_Ocean();
		break;
	case PLACE_ID::ENVI_PARK:
		hr = Loading_For_Envi_Park();
		break;
	case PLACE_ID::ENVI_PLAYGROUND:
		hr = Loading_For_Envi_Playground();
		break;
	case PLACE_ID::ENVI_CAFE:
		hr = Loading_For_Envi_Cafe();
		break;
	case PLACE_ID::ETC_COMMUINTY:
		hr = Loading_For_Etc_Community();
		break;
	case PLACE_ID::ETC_MUSIC:
		hr = Loading_For_Etc_Music();
		break;
	case PLACE_ID::ETC_RANKING:
		hr = Loading_For_Etc_Ranking();
		break;
	case PLACE_ID::ETC_NEWS:
		hr = Loading_For_Etc_News();
		break;
	case PLACE_ID::ETC_PHOTO:
		hr = Loading_For_Etc_Photo();
		break;
	case PLACE_ID::CONNECT_TENT:
		hr = Loading_For_Connect_Tent();
		break;
	case PLACE_ID::CONNECT_PORT:
		hr = Loading_For_Connect_Port();
		break;
	case PLACE_ID::LIVE_APARTMENT:
		hr = Loading_For_Live_Apartment();
		break;
	case PLACE_ID::LIVE_WEDDING_HOUSE:
		hr = Loading_For_Live_Wedding_House();
		break;
	case PLACE_ID::END:
		break;
	case PLACE_ID::ISLAND:
		hr = Loading_Island();
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

// Loading For Main Place // ===========================================

HRESULT CLoader::Loading_For_Main()
{
	CObject* tmp = { nullptr };

// Actor Loading //



	// Island //
	tmp = CIsland::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Actor_Island"),
		tmp)))
		return E_FAIL;


	tmp = CMii::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Actor_Mii"),
		tmp)))
		return E_FAIL;


	lstrcpy(m_szFPS, TEXT("Actor Loading Complete"));

// UI Loading //



	// Food //
	tmp = CFood::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_UI_Food"),
		tmp)))
		return E_FAIL;


	tmp = CPlaceMoveButton::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_UI_PlaceMoveButton"),
		tmp)))
		return E_FAIL;

	lstrcpy(m_szFPS, TEXT("UI Loading Complete"));

//Components Loading // ====================================================

	//Vtx_Tex Shader Component //
	tmp = CShaderCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Tex.hlsl"),
		Engine::VTX_TEX::Elements,
		Engine::VTX_TEX::iNumElements);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Vtx_TexShader"),
		tmp)))
		return E_FAIL;

	 
	//Normal Shader Component //
	tmp = CShaderCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Normal_Tex.hlsl"),
		Engine::VTX_TEX_NORMAL::Elements,
		Engine::VTX_TEX_NORMAL::iNumElements);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Vtx_Normal_TexShader"),
		tmp)))
		return E_FAIL;

	//Mesh Shader Component //
	tmp = CShaderCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Mesh.hlsl"),
		Engine::VTX_TEX_MESH::Elements,
		Engine::VTX_TEX_MESH::iNumElements);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_Vtx_Mesh_TexShader"),
		tmp)))
		return E_FAIL;


	// Rect Buffer Component //
	tmp = CRectBufferCom::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_RectBuffer"),
		tmp)))
		return E_FAIL;
	

	// Terrain Buffer Component // 
	tmp = CTerrainBufferCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/Terrain/Height.bmp"));

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_TerrainBuffer"),
		tmp)))
		return E_FAIL;
	


	// Food Texture Component //
	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/item_Food102.png"),
		1);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_FoodTexture"),
		tmp)))
		return E_FAIL;


	// Island Diffuse Texture Component //
	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/Terrain/Tile%d.dds"),
		2);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_IslandTexture"),
		tmp)))
		return E_FAIL;

	// Island Mask Texture Component //
	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/Terrain/Mask.bmp"),
		1);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_IslandMaskTexture"),
		tmp)))
		return E_FAIL;


	// Button Texture Component //
	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/UI_Buttons.png"),
		1);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_UIButtonTexture"),
		tmp)))
		return E_FAIL;


#define boatPath "../../../../../../Resource/main/model/Object - Boat/boat.fbx"
#define babyChairPath "../../../../../../Resource/main/model/Object - BabyChair/babyChair.fbx"
#define beachChairPath "../../../../../../Resource/main/model/Object - BeachChair/beachChair.fbx"

#define path1 "../../Resource/00Model/00Body/Cat1.fbx"
#define path2 "../../Resource/Models/Fiona/Fiona.fbx"
#define path3 "../../Resource/Models/A.fbx"

#define path4 "../../../../../../Resource/dev/model/02Arabian/room1.fbx"
#define path5 "../../../../../../Resource/dev/model/Canoe/Boat.fbx"
#define path6 "../../../../../../Resource/dev/model/BabyChair/BabyChair.fbx"
#define path7 "../../../../../../Resource/dev/model/Beach Chair/BeachChair.fbx"
#define path8 "../../../../../../Resource/dev/model/Cats/catAddAnim.fbx"
#define path9 "../../../../../../Resource/dev/model/Island - Island/Background Sky and Sea/Clouds/Cloud.fbx"
#define path10 "../../../../../../Resource/dev/model/Island - Island/Background Sky and Sea/Moon/Moon.fbx"
#define path11 "../../../../../../Resource/dev/model/Island - Island/Background Sky and Sea/Sky and Sea/Sea.fbx"
#define path12 "../../../../../../Resource/dev/model/Island - Island/House Plots/houseGround.fbx"
#define path13 "../../../../../../Resource/dev/model/Island - Island/Map Spring/island.fbx"
#define path14 "../../../../../../Resource/dev/model/Mii/Mii/body_body000.fbx"
#define path15 "../../../../../../Resource/dev/model/Mii/Mii/miiDeleteLeafBones.fbx"
#define path16 "../../../../../../Resource/dev/model/Mii News/Mii News/news.fbx"
#define path17 "../../../../../../Resource/dev/model/BackGround- Island/Map Spring/island.fbx"
	
	XMMATRIX initMatrix = XMMatrixIdentity();

	initMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

	// Test Model Component //
	tmp = CModel::Create(
		m_pDevice,
		m_pContext,
		path16,
		initMatrix);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Component_TestModel"),
		tmp)))

		return E_FAIL;


	lstrcpy(m_szFPS, TEXT("Component Loading Complete"));

// Loading Complete // ====================================================
	lstrcpy(m_szFPS, TEXT("Main Place Loading Complete"));
	m_isFinished = true;

	return S_OK;
}













// Loading For Setting Place // =========================================

HRESULT CLoader::Loading_For_Setting()
{
	CObject* tmp = { nullptr };

	// Island //
	tmp = CIsland::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Actor_Island"),
		tmp)))
		return E_FAIL;


	lstrcpy(m_szFPS, TEXT("Actor Loading Complete"));

	// UI Loading // ====================================================



		// Food //
	tmp = CFood::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_UI_Food"),
		tmp)))
		return E_FAIL;



	

	lstrcpy(m_szFPS, TEXT("UI Loading Complete"));

	//Components Loading // ====================================================

		//Vtx_Tex Shader Component //
	tmp = CShaderCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Tex.hlsl"),
		Engine::VTX_TEX::Elements,
		Engine::VTX_TEX::iNumElements);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Component_Vtx_TexShader"),
		tmp)))
		return E_FAIL;


	//Normal Shader Component //
	tmp = CShaderCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Normal_Tex.hlsl"),
		Engine::VTX_TEX_NORMAL::Elements,
		Engine::VTX_TEX_NORMAL::iNumElements);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Component_Vtx_Normal_TexShader"),
		tmp)))
		return E_FAIL;



	// Rect Buffer Component //
	tmp = CRectBufferCom::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Component_RectBuffer"),
		tmp)))
		return E_FAIL;


	// Terrain Buffer Component // 
	tmp = CTerrainBufferCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/Terrain/Height.bmp"));

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Component_TerrainBuffer"),
		tmp)))
		return E_FAIL;



	// Food Texture Component //
	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/item_Food102.png"),
		1);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Component_FoodTexture"),
		tmp)))
		return E_FAIL;


	// Island Texture Component //
	tmp = CTextureCom::Create(
		m_pDevice,
		m_pContext,
		TEXT("../../Resource/Textures/Terrain/Tile%d.dds"),
		2);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Component_IslandTexture"),
		tmp)))
		return E_FAIL;



	// Loading Complete // ====================================================
	lstrcpy(m_szFPS, TEXT("Setting Loading Complete"));
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
HRESULT CLoader::Loading_Island()
{
	CObject* tmp = { nullptr };

//Component
 	{
		// ProtoTag_Component_Island_Model
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

			tmp = CModel::Create(
				m_pDevice,
				m_pContext,
				"../../Resource/00Model/BackGround - Island/island.fbx",
				initMatrix);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACE_ID::ISLAND),
				TEXT("ProtoTag_Component_Island_Model"),
				tmp)))
				return E_FAIL;
		}

		// ProtoTag_Component_Mesh_Shader
		{
			tmp = CShaderCom::Create(
				m_pDevice,
				m_pContext,
				TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Mesh.hlsl"),
				VTX_TEX_MESH::Elements,
				VTX_TEX_MESH::iNumElements);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACE_ID::ISLAND),
				TEXT("ProtoTag_Component_Mesh_Shader"),
				tmp)))
				return E_FAIL;
		}
	}


	


	// Actor
	{
		// ProtoTag_Actor_Island
		{
			tmp = CIsland::Create(
				m_pDevice,
				m_pContext);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACE_ID::ISLAND),
				TEXT("ProtoTag_Actor_Island"),
				tmp)))
				return E_FAIL;
		}
	}
	// UI
	lstrcpy(m_szFPS, TEXT("Place Island Loading Complete"));
	m_isFinished = true;

	return S_OK;
}
#pragma endregion

CLoader* CLoader::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	PLACE_ID NextPlaceID)
{
	CLoader* pLoader = new CLoader(pDevice, pContext);

	if (FAILED(pLoader->Initialize(NextPlaceID)))
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
