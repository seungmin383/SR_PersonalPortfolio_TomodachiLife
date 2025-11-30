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

	// Component
	{
		/* 로딩 텍스쳐 */
		// ProtoTag_Component_LoadingTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/UIBackGround/Loading.dds"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iLoading, TEXT("ProtoTag_Component_LoadingTexture"), tmp)))
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
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iLoading, TEXT("ProtoTag_UI_LoadingBackGround"), tmp)))
				return E_FAIL;
		}
	}

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
		/* 공사중 표지판 */
		// ProtoTag_Component_Building0
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building0", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building0/Building.fbx", initMatrix, "Building0");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Building0"),tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building0", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype( static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_seBuilding0"), tmp)))
				return E_FAIL;
		}
		/* 아파트 */
		// ProtoTag_Component_Building1
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building1", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building1/Building.fbx", initMatrix, "Building1");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),TEXT("ProtoTag_Component_Building1"),tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building1", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_seBuilding1"), tmp)))
				return E_FAIL;
		}
		/* 음식가게 */
		// ProtoTag_Component_Building1
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building4", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building4/Building.fbx", initMatrix, "Building4");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Building4"), tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building4", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_seBuilding4"), tmp)))
				return E_FAIL;
		}
		/* 주민센터 */
		// ProtoTag_Component_Building10
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building10", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building10/Building.fbx", initMatrix, "Building10");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Building10"), tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building10", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_seBuilding10"), tmp)))
				return E_FAIL;
		}
		/* 뉴스 센터 */
		// ProtoTag_Component_Building19
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building19", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building19/Building.fbx", initMatrix, "Building19");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Building19"), tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building19", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_seBuilding19"), tmp)))
				return E_FAIL;
		}
		/* 카페 */
		// ProtoTag_Component_Building21
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_Building21", ANIM_TYPE::NONEANIM,
				"../../Resource/00Model/Actor - Building/Building21/Building.fbx", initMatrix, "Building21");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Building21"), tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Building21", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_seBuilding21"), tmp)))
				return E_FAIL;
		}
		/* 건물 버튼 텍스쳐 */
		// ProtoTag_Component_PlaceMoveButton_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/PlaceMoveButton/PlaceMoveButtonTexture%d.dds"), 46);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_PlaceMoveButton_Texture"), tmp)))
				return E_FAIL;
		}
		/* 건물 버튼 아웃라인 텍스쳐*/
		// ProtoTag_Component_OutLine_Texture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Textures/OutLine/OutLine%d.dds"), 6);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::ISLAND),
				TEXT("ProtoTag_Component_OutLine_Texture"), tmp)))
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
	}

	// Actor
	{
		// ProtoTag_Actor_Mii
		{
			tmp = CTestMii::Create(m_pDevice,m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				iAPART,
				TEXT("ProtoTag_Actor_Mii"),
				tmp)))
				return E_FAIL;
		}

		// ProtoTag_Actor_Player_Body
		{
			tmp = CBody::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				iAPART, 
				TEXT("ProtoTag_Actor_Player_Body"), 
				tmp)))
				return E_FAIL;
		}

		// ProtoTag_Actor_Player_Weapon
		{
			tmp = CWeapon::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				iAPART,
				TEXT("ProtoTag_Actor_Player_Weapon"),
				tmp)))
				return E_FAIL;
		}

		/* 플레이어 */
		// ProtoTag_Actor_Player
		{
			tmp = CPlayer::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				iAPART, 
				TEXT("ProtoTag_Actor_Player"), 
				tmp)))
				return E_FAIL;
		}
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
		// ProtoTag_Component_CMButtonTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/CommonButtonTexture/%d.dds"), 6);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_CMButtonTexture"), tmp)))
				return E_FAIL;
		}

		/* 아이콘 텍스쳐 */
		// ProtoTag_Component_IconTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/IconTexture/%d.dds"), 6);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_IconTexture"), tmp)))
				return E_FAIL;
		}

		/* UI 배경 텍스쳐 */
		// ProtoTag_Component_UIBGTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/MiiEditor/UIBG.png"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_UIBGTexture"), tmp)))
				return E_FAIL;
		}

		/* mii 편집 버튼 */
		// ProtoTag_Component_EditButtonTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/MiiEditor/EditButton/%d.dds"), 262);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_EditButtonTexture"), tmp)))
				return E_FAIL;
		}

		/* 페이지 변경 버튼 */
		// ProtoTag_Component_PageButtonTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/MiiEditor/PageButton/%d.dds"), 32);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_PageButtonTexture"), tmp)))
				return E_FAIL;
		}

		/* 텍스트 박스 */
		// ProtoTag_Component_TextBoxTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/TextBox/%d.dds"), 1);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_Component_TextBoxTexture"), tmp)))
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
		/* 플레이어 */
		// ProtoTag_Actor_Player
		{
			tmp = CPlayer::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL,TEXT("ProtoTag_Actor_Player"),tmp)))
				return E_FAIL;
		}
	}

	// UI
	{
		/* 일반 버튼 */
		// ProtoTag_UI_CMButton
		{
			tmp = CCommonButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_CMButton"), tmp)))
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

		/* 아래 UI 배경*/
		// ProtoTag_UI_BackGround
		{
			tmp = CBackGround::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_BackGround"), tmp)))
				return E_FAIL;
		}

		/* mii 편집 버튼 */
		// ProtoTag_UI_MiiEditButton
		{
			tmp = CMiiEditButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_MiiEditButton"), tmp)))
				return E_FAIL;
		}
		
		/* 페이지 버튼 */
		// ProtoTag_UI_PageButton
		{
			tmp = CPageButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_PageButton"), tmp)))
				return E_FAIL;
		}

		/* 텍스트 버튼 */
		// ProtoTag_UI_TextButton
		{
			tmp = CTextButton::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iCITYHALL, TEXT("ProtoTag_UI_TextButton"), tmp)))
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

	// Component
	{
		/* Mii 에디터 배경 모델 */
		// ProtoTag_Component_EditorBG
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);

			tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_EditorBG", ANIM_TYPE::ANIM,
				"../../Resource/00Model/BackGround - MiiEditor/EditorBG.fbx", initMatrix, "EditorBG");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_Component_EditorBG"), tmp)))
				return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "EditorBG", initMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_Component_seEditorBG"), tmp)))
				return E_FAIL;
		}
	}

	// Actor
	{	
		// ProtoTag_Actor_EditorBG
		{
			tmp = CMiiEditorBG::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_Actor_EditorBG"), tmp)))
				return E_FAIL;
		}
	}

	// UI
	{
		// ProtoTag_UI_PlaceMoveButton
		{
			tmp = CPlaceMoveButton::Create(m_pDevice, m_pContext);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_UI_PlaceMoveButton"), tmp)))
				return E_FAIL;
		}
	}

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));
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
				"../../../../../../Resource/dev/05_Anim_fbx/CM_Run00.fbx", InitMatrix, "ChineseDress");
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_BasicManBody_Model"), tmp)))
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
