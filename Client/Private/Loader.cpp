#include "Apart.h"
#include "BackGround.h"
#include "Body.h"
#include "CommonButton.h"
#include "GameInstance.h"
#include "Island.h"
#include "Loader.h"
#include "MiiEditorBG.h"
#include "PlaceMoveButton.h"
#include "Player.h"
#include "SkyAndSea.h"
#include "TempModel.h"
#include "Terrain.h"
#include "PlayerInven_Button.h"
#include "TestMii.h"
#include "TextBox.h"
#include "UICamera.h"
#include "Weapon.h"
#include "WorldCamera.h"
#include "LevelBar.h"
#include "Gauge.h"
#include "Store_Food.h"
#include "MiiEditButton.h"
#include "Personal_Button.h"
#include "Save_Button.h"
#include "PageButton.h"
#include "Sky_Actor.h"
#include "Sea_Actor.h"
#include "Park_Actor.h"
#include "HungryBar.h"
#include "Hungry_Gauge.h"
#include "Collider.h"
#include "IconTexture.h"
#include "TextButton.h"
#include "Room.h"
#include "BuildingName_Texture.h"
#include "CityhallMenu_Button.h"
#include "Snow.h"
#include "Explosion.h"
#include "LevelUp_Inventory.h"
#include "LevelUp_Item.h"
#include "FoodItem_Button.h"
#include "Level_TextBox.h"
#include "Thinking_Button.h"
#include "FriendList_BackGround.h"
#include "Voice_Button.h"
#include "Friend_TextBox.h"
#include "EditOption_BackGround.h"

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

HRESULT CLoader::Initialize(PLACEID NextPlaceID)
{
	m_eNextPlaceID = NextPlaceID;

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
	case PLACEID::LOADING:
		hr = Loading_Loading();
		break;

	case PLACEID::ISLAND:
		hr = Loading_Island();
		break;

	case PLACEID::APART:
		hr = Loading_Apart();
		break;

	case PLACEID::CITYHALL:
		hr = Loading_CityHall();
		break;

	case PLACEID::MII_EDITOR:
		hr = Loading_MiiEditor();
		break;

	case PLACEID::ROOM:
		hr = Loading_Room();
		break;

	case PLACEID::FOOD_STORE:
		hr = Loading_FoodStore();
		break;

	case PLACEID::EVENT_BEFRIEND:
		hr = Loading_Event_BeFriend();
		break;

	case PLACEID::EVENT_LOVE:
		hr = Loading_Event_Love();
		break;

	case PLACEID::DEVELOPER:
		hr = Loading_Developer();
		break;

	case PLACEID::END:
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

HRESULT CLoader::Loading_Loading()
{
	CObject* tmp = { nullptr };
	uint32_t iLoading = static_cast<uint32_t>(PLACEID::LOADING);

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_Island()
{
	CObject* tmp = { nullptr };

	//Component
 	{
		/* 섬 */
		// ProtoTag_Component_Island_seModel
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix;// = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

			tmp = CSEModel::Create(
				m_pDevice,
				m_pContext,
				ANIM_TYPE::NONEANIM,
				"Island",
				initMatrix);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_Island_seModel"),
				tmp)))
				return E_FAIL;
		}
		/* 하늘이랑 바다 */
		// ProtoTag_Component_SkyAndSea_seModel
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix;// = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

			tmp = CSEModel::Create(
				m_pDevice,
				m_pContext,
				ANIM_TYPE::NONEANIM,
				"SkyAndSea",
				initMatrix);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_SkyAndSea_seModel"),
				tmp)))
				return E_FAIL;
		}

		/* 빌딩들 */
		for (uint32_t i = 0; i < 24; ++i)
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

			_wstring ProtoTagw = L"ProtoTag_Component_Building" + to_wstring(i);
			_wstring seProtoTag = L"ProtoTag_Component_seBuilding" + to_wstring(i);
			string FilePath = "../../Resource/00Model/Actor - Building/Building" + to_string(i) + "/Building.fbx";
			string SaveName = "Building" + to_string(i);

#ifdef _DEBUG
			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM,
				FilePath.c_str(), initMatrix, SaveName.c_str());
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), ProtoTagw, tmp)))
				return E_FAIL;
#endif

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, SaveName.c_str(), initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), seProtoTag, tmp)))
				return E_FAIL;
		}
		

		/* 하늘 모델 */
		// ProtoTag_Component_Sky
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);

#ifdef _DEBUG
			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "../../Resource/Model_Environment/Sky/Sky.fbx", initMatrix, "Sky");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Sky_Model"), tmp)))
				return E_FAIL;
#endif
			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "Sky", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Sky_SeModel"), tmp)))
				return E_FAIL;
		}

		/* 바다 모델 */
		// ProtoTag_Component_Sky
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

#ifdef _DEBUG
			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "../../Resource/Model_Environment/Sea/Sea.fbx", initMatrix, "Sea");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Sea_Model"), tmp)))
				return E_FAIL;
#endif
			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "Sea", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Sea_SeModel"), tmp)))
				return E_FAIL;
		}

		/* 건물 버튼 텍스쳐 */
		// ProtoTag_Component_PlaceMoveButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/PlaceMarker_Button/%d.dds"), 46);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_PlaceMarker_Button_Texture"), tmp)))
				return E_FAIL;
		}

		/* 건물 버튼 아웃라인 텍스쳐*/
		// ProtoTag_Component_OutLine_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/PlaceMarker_Button/Outline/%d.dds"), 6);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_PlaceMarker_Button_OutLine_Texture"), tmp)))
				return E_FAIL;
		}

		/* 건물 이름 텍스쳐*/
		// ProtoTag_Component_OutLine_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/BuildingName_Texture/%d.png"), 25);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_BuildingName_Texture"), tmp)))
				return E_FAIL;
		}
		

		/* 아래 UI 배경 */
		// ProtoTag_Component_BackGround_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/UIBackGorund/UIBackGround.png"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_BackGround_Texture"), tmp)))
				return E_FAIL;
		}
		/* 뒤로가기 같은 버튼 텍스쳐 */
		// ProtoTag_Component_CommonButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/CommonButton/CommonButtonTexture%d.png"), 3);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_CommonButton_Texture"), tmp)))
				return E_FAIL;
		}

		/* 저장 버튼 텍스쳐 */
		// ProtoTag_Component_Save_Button_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Save_Button/%d.dds"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_Save_Button_Texture"), tmp)))
				return E_FAIL;
		}


	}

	// Actor
	{
		/* 섬 */
		// ProtoTag_Actor_Island
		{
			tmp = CIsland::Create(m_pDevice,m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Actor_Island"), tmp)))
				return E_FAIL;
		}
		/* 하늘이랑 바다 */
		// ProtoTag_Actor_SkyAndSea
		{
			tmp = CSkyAndSea::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Actor_SkyAndSea"), tmp)))
				return E_FAIL;
		}
		/* 건물들 */
		// ProtoTag_Actor_TempModel
		{
			tmp = CTempModel::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Actor_TempModel"), tmp)))
				return E_FAIL;
		}

		/* 하늘 */
		//ProtoTag_Actor_Sky
		{
			tmp = CSky_Actor::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Actor_Sky"), tmp)))
				return E_FAIL;
		}

		/* 바다 */
		//ProtoTag_Actor_Sea
		{
			tmp = CSea_Actor::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Actor_Sea"), tmp)))
				return E_FAIL;
		}
	}
	
	// UI
	{
		/* 건물 버튼 */
		// ProtoTag_UI_PlaceMoveButton
		{
			tmp = CPlaceMoveButton::Create(m_pDevice,m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_UI_PlaceMoveButton"), tmp)))
				return E_FAIL;
		}

		/* 아래 UI 배경*/
		// ProtoTag_UI_BackGround
		{
			tmp = CBackGround::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_UI_BackGround"), tmp)))
				return E_FAIL;
		}

		/* 빌딩 이름 텍스쳐 UI */
		// ProtoTag_UI_BuildingNameTexture
		{
			tmp = CBuildingName_Texture::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_UI_BuildingNameTexture"), tmp)))
				return E_FAIL;
		}

		/* 세이브 버튼 */
		// ProtoTag_UI_Save_Button
		{
			tmp = CSave_Button::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_UI_Save_Button"), tmp)))
				return E_FAIL;
		}


	}

	lstrcpy(m_szFPS, TEXT("Place Island Loading Complete"));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_Apart()
{
	CObject* tmp = { nullptr };
	uint32_t iAPART = static_cast<uint32_t>(PLACEID::APART);

	// Component
	{
		// ProtoTag_Component_GirlHair1_Model
		{
			XMMATRIX InitMatrix;
			InitMatrix = XMMatrixIdentity();
			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM,
				"../../Resource/00Model/00Body/CamouflageJacket/Untitled.fbx", InitMatrix, "Hair1");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Component_GirlHair1_Model"), tmp)))
				return E_FAIL;
		}

		/* 방 모델 */
		// ProtoTag_Component_Room
		{
			for (uint32_t i = 0; i < 3; ++i)
			{
				XMMATRIX InitMatrix = XMMatrixIdentity();

				_wstring wstrProtoTag = L"ProtoTag_Component_Room" + to_wstring(i);
				_wstring wstrSeProtoTag = L"ProtoTag_Component_seRoom" + to_wstring(i);
				_string strFilePath = "../../Resource/Actor/room" + to_string(i) + "/room.fbx";
				_string strSaveName = "room" + to_string(i);

#ifdef _DEBUG
				tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, strFilePath.c_str(), InitMatrix, strSaveName.c_str());
				if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, wstrProtoTag, tmp)))
					return E_FAIL;
#endif

				tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM,strSaveName.c_str(), InitMatrix);
				if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, wstrSeProtoTag, tmp)))
					return E_FAIL;
			}
		}		

		// ProtoTag_Component_seBuilding1
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

#ifdef _DEBUG
			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building1/Building.fbx", initMatrix, "Building1");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Component_Building1") , tmp)))
				return E_FAIL;
#endif 
			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building1", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Component_seBuilding1"), tmp)))
				return E_FAIL;
		}

		/* 일반 버튼 텍스쳐 */
		// ProtoTag_Component_CMButtonTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/CommonButtonTexture/%d.dds"), 6);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Component_CMButtonTexture"), tmp)))
				return E_FAIL;
		}
	}

	// Actor
	{
		/* 주민 방 */
		// ProtoTag_Actor_Room
		{
			tmp = CRoom::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Actor_Room"), tmp)))
				return E_FAIL;
		}

		/* 아파트 */
		// ProtoTag_Actor_Apart
		{
			tmp = CApart::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Actor_Apart"), tmp)))
				return E_FAIL;
		}
	}

	/* 일반 버튼 */
	// ProtoTag_UI_CMButton
	{
		tmp = CCommonButton::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_UI_CMButton"), tmp)))
			return E_FAIL;
	}
	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_CityHall()
{
	CObject* tmp = { nullptr };
	uint32_t iCITYHALL = static_cast<uint32_t>(PLACEID::CITYHALL);

	// Component
	{
	}

	// Actor
	{
	}

	// UI
	{
		/* 마을회관 메뉴 버튼 */
		// ProtoTag_UI_CMButton
		{
			tmp = CCityhallMenu_Button::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_CityhallMenu_Button"), tmp)))
				return E_FAIL;
		}
		
		/* 아이콘 */
		// ProtoTag_UI_Icon
		{
			tmp = CIconTexture::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_Icon"), tmp)))
				return E_FAIL;
		}
	}

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_MiiEditor()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

	// Component
	{
		/* UI 배경 텍스쳐 */
		// ProtoTag_Component_UI_BackGround_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/MiiEditor/UIBG.png"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_UI_BackGround_Texture"), tmp)))
				return E_FAIL;
		}

		/* mii 편집 버튼 텍스쳐 */
		// ProtoTag_Component_EditButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/MiiEdit_Button/%d.dds"), 262);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_EditButton_Texture"), tmp)))
				return E_FAIL;
		}

		/* 페이지 변경 버튼 텍스쳐 */
		// ProtoTag_Component_PageButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Page_Button/%d.dds"), 32);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_PageButton_Texture"), tmp)))
				return E_FAIL;
		}

		/* 텍스트 버튼 텍스쳐 */
		// ProtoTag_Component_TextButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Text_Button/%d.dds"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_TextButton_Texture"), tmp)))
				return E_FAIL;
		}

		/* 보이스 버튼 텍스쳐 */
		// ProtoTag_Component_VoiceButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Voice_Button/%d.dds"), 10);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VoiceButton_Texture"), tmp)))
				return E_FAIL;
		}

		/* 보이스 버튼 텍스쳐 */
		// ProtoTag_Component_VoiceButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Voice_Button/%d.dds"), 10);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VoiceButton_Texture1"), tmp)))
				return E_FAIL;
		}

		/* 성격 버튼 텍스쳐 */
		// ProtoTag_Component_PersonalButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Personal_Button/%d.dds"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_PersonalButton_Texture"), tmp)))
				return E_FAIL;
		}
	}

	// Actor
	{	
		
	}

	// UI
	{
		/* 아래 UI 배경*/
		// ProtoTag_UI_BackGround
		{
			tmp = CBackGround::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_BackGround"), tmp)))
				return E_FAIL;
		}


		/* mii 편집 버튼 */
		// ProtoTag_UI_MiiEditButton
		{
			tmp = CMiiEditButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), tmp)))
				return E_FAIL;
		}

		/* 페이지 버튼 */
		// ProtoTag_UI_PageButton
		{
			tmp = CPageButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_PageButton"), tmp)))
				return E_FAIL;
		}
		/* 텍스트 버튼 */
		// ProtoTag_UI_TextButton
		{
			tmp = CTextButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_TextButton"), tmp)))
				return E_FAIL;
		}

		/* 편집 옵션 배경 */
		// ProtoTag_UI_EditOption_BackGround
		{
			tmp = CEditOption_BackGround::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_EditOption_BackGround"), tmp)))
				return E_FAIL;
		}


		/* 마을회관 메뉴 버튼 */
		// ProtoTag_UI_CMButton
		{
			tmp = CCityhallMenu_Button::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_CityhallMenu_Button"), tmp)))
				return E_FAIL;
		}

		/* 아이콘 */
		// ProtoTag_UI_Icon
		{
			tmp = CIconTexture::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Icon"), tmp)))
				return E_FAIL;
		}

		/* 보이스 버튼 */
		// ProtoTag_UI_Voice_Button
		{
			tmp = CVoice_Button::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Voice_Button"), tmp)))
				return E_FAIL;
		}

		/* 성격 버튼 */
		// ProtoTag_UI_Personal_Button
		{
			tmp = CPersonal_Button::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Personal_Button"), tmp)))
				return E_FAIL;
		}
	}

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
	m_isFinished = true;
	return S_OK;
}


HRESULT CLoader::Loading_Room()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);
	/* Component */
	
	/* 레벨업 선물 인벤토리 텍스쳐 */
	// ProtoTag_Component_LevelUp_Inventory_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/LevelUP_Inventory_Texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_LevelUp_Inventory_Texture"), tmp)))
			return E_FAIL;
	}

	/* 레벨업 바 텍스쳐 */
	// ProtoTag_Component_LevelUpBar_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/LevelUpBar_Texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_LevelUpBar_Texture"), tmp)))
			return E_FAIL;
	}

	/* 레벨업 게이지 텍스쳐 */
	// ProtoTag_Component_LevelGauge_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/LevelUpBar_Texture/Gauge/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_LevelGauge_Texture"), tmp)))
			return E_FAIL;
	}


	/* 배고픔 바 텍스쳐 */
	// ProtoTag_Component_HungryBar_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Hungry_Texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_HungryBar_Texture"), tmp)))
			return E_FAIL;
	}


	/* 배고픔 게이지 텍스쳐 */
	// ProtoTag_Component_HungryGauge_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Hungry_Texture/Gauge/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_HungryGauge_Texture"), tmp)))
			return E_FAIL;
	}

	/* 텍스트 박스 텍스쳐 */
	// ProtoTag_Component_TextBox_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/TextBox_Texture/%d.dds"), 5);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_TextBox_Texture"), tmp)))
			return E_FAIL;
	}
	
	/* 생각하는 버튼 텍스쳐 */
	// ProtoTag_Component_Thinking_Button_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Thinking_Button/%d.dds"), 2);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Thinking_Button_Texture"), tmp)))
			return E_FAIL;
	}

	/* 친구 목록 뒷배경 텍스쳐 */
	// ProtoTag_Component_FriendList_BackGround_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/FriendList_BackGround_Texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_FriendList_BackGround_Texture"), tmp)))
			return E_FAIL;
	}

	/* Actor */



	/* UI */
	
	/* 레벨업 선물 인벤토리 */
	// ProtoTag_UI_LevelUp_Inventory
	{
		tmp = CLevelUp_Inventory::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_LevelUp_Inventory"), tmp)))
			return E_FAIL;
	}

	/* 레벨업 바 */
	// ProtoTag_UI_LevelBar
	{
		tmp = CLevelBar::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_LevelBar"), tmp)))
			return E_FAIL;
	}


	/* 레벨업 게이지 */
	// ProtoTag_UI_LevelGauge
	{
		tmp = CGauge::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_LevelGauge"), tmp)))
			return E_FAIL;
	}

	/* 배고픔 바 */
	// ProtoTag_UI_HungryBar
	{
		tmp = CHungryBar::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_HungryBar"), tmp)))
			return E_FAIL;
	}

	/* 배고픔 게이지 */
	// ProtoTag_UI_HungryGauge
	{
		tmp = CHungry_Gauge::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_HungryGauge"), tmp)))
			return E_FAIL;
	}

	/* 텍스트 박스 */
	// ProtoTag_UI_TextBox
	{
		tmp = CTextBox::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_TextBox"), tmp)))
			return E_FAIL;
	}

	/* 음식 버튼 */
	// ProtoTag_UI_FoodItem_Button
	{
		tmp = CFoodItem_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_FoodItem_Button"), tmp)))
			return E_FAIL;
	}

	/* 레벨 텍스트박스*/
	// ProtoTag_UI_Level_TextBox
	{
		tmp = CLevel_TextBox::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Level_TextBox"), tmp)))
			return E_FAIL;
	}


	/* 생각하는 버튼 */
	// ProtoTag_UI_Thinking_Button
	{
		tmp = CThinking_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Thinking_Button"), tmp)))
			return E_FAIL;
	}

	/* 친구 목록 뒷 배경*/
	// ProtoTag_UI_FriendList_BackGround
	{
		tmp = CFriendList_BackGround::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_FriendList_BackGround"), tmp)))
			return E_FAIL;
	}

	/* 친구 목록 */
	// ProtoTag_UI_Friend_TextBox
	{
		tmp = CFriend_TextBox::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Friend_TextBox"), tmp)))
			return E_FAIL;
	}

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_FoodStore()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	/* Component */

	/* 식품점 모델*/
	// ProtoTag_Component_FoodStore_Model
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
#ifdef _DEBUG
		tmp = CModel::Create(m_pDevice, m_pContext, 
			ANIM_TYPE::NONEANIM, "../../Resource/Model_Sell/Food/0.fbx", InitMatrix, "FoodStore");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_FoodStore_Model"), tmp)))
			return E_FAIL;
#endif 
		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "FoodStore", InitMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_FoodStore_seModel"), tmp)))
			return E_FAIL;
	}

	/* Actor */

	/* 식품점 */
	// ProtoTag_Actor_FoodStore
	{
		tmp = CStore_Food::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_FoodStore"), tmp)))
			return E_FAIL;
	}

	/* 주민 방 */
	// ProtoTag_Actor_Room
	{
		tmp = CRoom::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Room"), tmp)))
			return E_FAIL;
	}
	/* UI */

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_Event_BeFriend()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_BEFRIEND);



	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_Event_Love()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_LOVE);

	/* Component */

	/* 공원 모델 */
	// ProtoTag_Component_Park_SeModel
	{
		XMMATRIX initMatrix = XMMatrixIdentity();
		initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

#ifdef _DEBUG
		tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "../../Resource/Model_Environment/Park/Park.fbx", initMatrix, "Park");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Park_Model"), tmp)))
			return E_FAIL;
#endif
		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Park", initMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Park_SeModel"), tmp)))
			return E_FAIL;
	}


	/* UI */


	/* Actor */

	/* 공원 */
	// ProtoTag_Actor_Park
	{
		tmp = CPark_Actor::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Park"), tmp)))
			return E_FAIL;
	}


	m_isFinished = true;
	return S_OK;
}


HRESULT CLoader::Loading_Developer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::DEVELOPER);

	// Component
	{
		/* VTXNOR 쉐이더 */
		// ProtoTag_Component_VtxNormalMeshShader
		{
			tmp = CShaderCom::Create(m_pDevice,m_pContext,TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Normal_Tex.hlsl"),
				Engine::VTX_TEX_NORMAL::Elements,Engine::VTX_TEX_NORMAL::iNumElements);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VtxNormalMeshShader"), tmp)))
				return E_FAIL;
		}

		/* 터레인 마스크 텍스쳐 */
		// ProtoTag_Component_IslandMaskTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/Terrain/Mask.bmp"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_IslandMaskTexture"), tmp)))
					return E_FAIL;
		}
		/* 터레인 텍스쳐 */
		// ProtoTag_Component_IslandTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/Terrain/Tile%d.dds"), 2);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_IslandTexture"), tmp)))
				return E_FAIL;
		}
		/* 터레인 버퍼 */
		// Terrain Buffer Component // 
		{
			tmp = CTerrainBufferCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/Terrain/Height.bmp"));
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_TerrainBuffer"), tmp)))
				return E_FAIL;
		}


		// ProtoTag_Component_BasicManBody_SEModel
		{
			XMMATRIX InitMatrix;
			InitMatrix = XMMatrixIdentity();
			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "ChineseDress", InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_BasicManBody_SEModel"), tmp)))
				return E_FAIL;
		}

		/* 콜라이더 */
		// ProtoTag_Component_AABBCollider
		{
			tmp = CCollider::Create(m_pDevice, m_pContext, COLLIDER::AABB);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_AABBCollider"), tmp)))
				return E_FAIL;

			tmp = CCollider::Create(m_pDevice, m_pContext, COLLIDER::OBB);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_OBBCollider"), tmp)))
				return E_FAIL;

			tmp = CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_SPHERECollider"), tmp)))
				return E_FAIL;
		}

		/* 눈 텍스쳐 */
		// ProtoTag_Component_Snow_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/Snow.png"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Snow_Texture"), tmp)))
				return E_FAIL;
		}

	}

	// Actor
	{
		/* 터레인 */
		// ProtoTag_Actor_Terrain
		{
			tmp = CTerrain::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Terrain"), tmp)))
				return E_FAIL;
		}

		/* 눈 */
		// ProtoTag_Actor_Snow
		{
			tmp = CSnow::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Snow"), tmp)))
				return E_FAIL;
		}

		/* 폭발 */
		// ProtoTag_Actor_Snow
		{
			tmp = CExplosion::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Explosion"), tmp)))
				return E_FAIL;
		}
	}
	// UI
	{
		// ProtoTag_UI_CommonButton
		{
			tmp = CCommonButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_CommonButton"), tmp)))
				return E_FAIL;
		}
	}
	lstrcpy(m_szFPS, TEXT("로딩 끝"));
	m_isFinished = true;
	return S_OK;
}

#pragma endregion

CLoader* CLoader::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	PLACEID NextPlaceID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);
	if (FAILED(pInstance->Initialize(NextPlaceID)))
	{
		MSG_BOX("ErrCode : Loader Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
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
