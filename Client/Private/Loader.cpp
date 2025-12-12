#include "Apart.h"
#include "BackGround.h"
#include "Body.h"
#include "CommonButton.h"
#include "Food.h"
#include "GameInstance.h"
#include "Island.h"
#include "Loader.h"
#include "MiiEditorBG.h"
#include "PlaceMoveButton.h"
#include "Player.h"
#include "SkyAndSea.h"
#include "TempModel.h"
#include "Terrain.h"
#include "TestMii.h"
#include "UICamera.h"
#include "Weapon.h"
#include "WorldCamera.h"
#include "MiiEditButton.h"
#include "PageButton.h"
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
		 
		//// ProtoTag_Component_Island_Model
		//{
		//	XMMATRIX initMatrix = XMMatrixIdentity();
		//	initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

		//	tmp = CModel::Create(
		//		m_pDevice,
		//		m_pContext,
		//		"ProtoTag_Component_Island_Model",
		//		ANIM_TYPE::NONEANIM,
		//		"../../Resource/00Model/BackGround - Island/island.fbx",
		//		initMatrix,
		//		"Island");

		//	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		//		static_cast<uint32_t>(PLACEID::ISLAND),
		//		TEXT("ProtoTag_Component_Island_Model"),
		//		tmp)))
		//		return E_FAIL;
		//}
		
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

			string ProtoTag = "ProtoTag_Component_Building" + to_string(i);
			_wstring ProtoTagw = L"ProtoTag_Component_Building" + to_wstring(i);
			_wstring seProtoTag = L"ProtoTag_Component_seBuilding" + to_wstring(i);
			string FilePath = "../../Resource/00Model/Actor - Building/Building" + to_string(i) + "/Building.fbx";
			string SaveName = "Building" + to_string(i);

			tmp = CModel::Create(m_pDevice, m_pContext, ProtoTag, ANIM_TYPE::NONEANIM,
				FilePath.c_str(), initMatrix, SaveName.c_str());
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), ProtoTagw, tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, SaveName.c_str(), initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), seProtoTag, tmp)))
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
		// ProtoTag_Component_BasicManBody_Model
		{
			XMMATRIX InitMatrix;
			InitMatrix = XMMatrixIdentity();
			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_BasicManBody_Model", ANIM_TYPE::ANIM,
				"../../Resource/Models/Fiona/Fiona.fbx", InitMatrix, "BasicManBody");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Component_BasicManBody_Model"), tmp)))
				return E_FAIL;
		}

		// ProtoTag_Component_GirlHair1_Model
		{
			XMMATRIX InitMatrix;
			InitMatrix = XMMatrixIdentity();
			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_GirlHair1_Model", ANIM_TYPE::ANIM,
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

				_string strProtoTag = "ProtoTag_Component_Room" + to_string(i);
				_wstring wstrProtoTag = L"ProtoTag_Component_Room" + to_wstring(i);
				_wstring wstrSeProtoTag = L"ProtoTag_Component_seRoom" + to_wstring(i);
				_string strFilePath = "../../Resource/Actor/room" + to_string(i) + "/room.fbx";
				_string strSaveName = "room" + to_string(i);

				tmp = CModel::Create(m_pDevice, m_pContext, strProtoTag, ANIM_TYPE::ANIM, strFilePath.c_str(), InitMatrix, strSaveName.c_str());
				if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, wstrProtoTag, tmp)))
					return E_FAIL;

				tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM,
					strSaveName.c_str(), InitMatrix, "Action");
				if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, wstrSeProtoTag, tmp)))
					return E_FAIL;
			}
		}		

		// ProtoTag_Component_seBuilding1
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);


			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building1" , ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building1/Building.fbx", initMatrix, "Building1");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Component_Building1") , tmp)))
				return E_FAIL;

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
		// ProtoTag_Actor_Mii
		{
			tmp = CTestMii::Create(m_pDevice,m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Actor_Mii"), tmp)))
				return E_FAIL;
		}

		// ProtoTag_Actor_Player_Body
		{
			tmp = CBody::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Actor_Player_Body"), tmp)))
				return E_FAIL;
		}

		/* 무기 */
		// ProtoTag_Actor_Player_Weapon
		{
			tmp = CWeapon::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Actor_Player_Weapon"), tmp)))
				return E_FAIL;
		}

		/* 플레이어 */
		// ProtoTag_Actor_Player
		{
			tmp = CPlayer::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iAPART, TEXT("ProtoTag_Actor_Player"), tmp)))
				return E_FAIL;
		}

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
		/* 일반 버튼 텍스쳐 */
		// ProtoTag_Component_CityhallMenu_Button_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/CityhallMenu_Button/%d.dds"), 2);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_CityhallMenu_Button_Texture"), tmp)))
				return E_FAIL;
		}

		/* 아이콘 텍스쳐 */
		// ProtoTag_Component_IconTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/CityhallMenu_Button/Icon/%d.dds"), 8);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_IconTexture"), tmp)))
				return E_FAIL;
		}


		/* 배경 모델 */
		// ProtoTag_Componenet_MiiEditorBG
		{
			XMMATRIX InitMatrix = XMMatrixIdentity();
			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Componenet_MiiEditorBG", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/BackGround - MiiEditor/EditorBG.fbx", InitMatrix, "EditorBG");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Componenet_MiiEditorBG"), tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "EditorBG", InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Componenet_seMiiEditorBG"), tmp)))
				return E_FAIL;
		}

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

		/* 에디터 배경 */
		// ProtoTag_Actor_EditorBG
		{
			tmp = CMiiEditorBG::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Actor_EditorBG"), tmp)))
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
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/LevelUP_Inventory_texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_LevelUp_Inventory_Texture"), tmp)))
			return E_FAIL;
	}



	/* Actor */

	/* 주민 방 */
	// ProtoTag_Actor_Room
	{
		tmp = CRoom::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Room"), tmp)))
			return E_FAIL;
	}




	/* UI */
	
	/* 레벨업 선물 인벤토리 */
	// ProtoTag_UI_LevelUp_Inventory
	{
		tmp = CLevelUp_Inventory::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_LevelUp_Inventory"), tmp)))
			return E_FAIL;
	}

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
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

		/* 테스트 모델 */
		// ProtoTag_Component_BasicManBody_Model
		{
			XMMATRIX InitMatrix;
			InitMatrix = XMMatrixIdentity();

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_BasicManBody_Model", ANIM_TYPE::ANIM,
				"../../../../../../Resource/dev/05_Anim_fbx/CM_Greet00.fbx", InitMatrix, "ChineseDress");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_BasicManBody_Model"), tmp)))
				return E_FAIL;
		}

		// ProtoTag_Component_BasicManBody_SEModel
		{
			XMMATRIX InitMatrix;
			InitMatrix = XMMatrixIdentity();
			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "ChineseDress", InitMatrix, "CM_Wait00");
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
		/* 테스트 mii*/
		// ProtoTag_Actor_Fiona
		{
			tmp = CTestMii::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Fiona"), tmp)))
				return E_FAIL;
		}
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
