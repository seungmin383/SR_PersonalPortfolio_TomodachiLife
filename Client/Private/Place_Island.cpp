#include "Place_Island.h"
#include "GameInstance.h"
#include "Place_Loading.h"


#include"MiiManager.h"
#include "ApartManager.h"


/* Component */

/* Actor */
#include "WorldCamera.h"
#include "UICamera.h"
#include "Island.h"
#include "SkyAndSea.h"
#include "Apart.h"
#include "TempModel.h"
#include "Sky_Actor.h"
#include "Sea_Actor.h"

/* UI */
#include "BackGround.h"
#include "PlaceMoveButton.h"
#include "YellowBackGround.h"
#include "BuildingName_Texture.h"
#include "Save_Button.h"
#include "IconTexture.h"
#include "Back_Button.h"
#include "Wallet.h"
#include "Window_Button.h"

/* Strgy*/
#include "IUnlock.h"
#include "ChangePlace_Strategy.h"
#include "CameraZoom_Strgy.h"
#include "Deactive_Strgy.h"
#include "ControlEnable_Strgy.h"


/***********************
 * ISLAND PLACE POS / TARGET POS
 ***********************/

 /* 음식점 */
#define FOODSTORE_POS        XMFLOAT4{-7.47224f, -0.561126f, -10.7086f, 1.f}
#define FOODSTORE_TARGETPOS  XMFLOAT4{-7.47224f,  0.561126f, -10.7086f, 1.f}

/* 아파트 (y += 1.9f) */
#define APART_POS            XMFLOAT4{0.f, 0.3f, 0.f, 1.f}
#define APART_TARGETPOS      XMFLOAT4{0.f, 2.2f, 0.f, 1.f}

/* 주민센터 (y += 2.f) */
#define CITYHALL_POS         XMFLOAT4{4.93322f, 0.0844781f, -2.84163f, 1.f}
#define CITYHALL_TARGETPOS   XMFLOAT4{4.93322f, 2.0844781f, -2.84163f, 1.f}

/* 뉴스 스테이션 */
#define NEWSSTATION_POS      XMFLOAT4{23.f, 0.1f, 20.9f, 1.f}
#define NEWSSTATION_TARGETPOS XMFLOAT4{23.f, 5.1f, 20.9f, 1.f}

/* 옷가게 (Wear Store) */
#define WEARSTORE_POS        XMFLOAT4{-6.96466f, 2.51501f, 4.36426f, 1.f}
#define WEARSTORE_TARGETPOS  XMFLOAT4{-6.96466f, 3.51501f, 4.36426f, 1.f}

/* 전당포 (Antique) */
#define ANTIQUE_POS          XMFLOAT4{-8.69427f, 1.95937f, -0.769943f, 1.f}
#define ANTIQUE_TARGETPOS    XMFLOAT4{-8.69427f, 2.95937f, -0.769943f, 1.f}

/* 모자가게 (Cap Store) */
#define CAPSTORE_POS         XMFLOAT4{-6.23547f, 0.938355f, -5.98064f, 1.f}
#define CAPSTORE_TARGETPOS   XMFLOAT4{-6.23547f, 1.938355f, -5.98064f, 1.f}

/* 음악당 (Music Hall) */
#define MUSICHALL_POS        XMFLOAT4{1.64f, -0.98f, -7.95f, 1.f}
#define MUSICHALL_TARGETPOS  XMFLOAT4{1.64f,  0.02f, -7.95f, 1.f}

/* 인테리어 / 집가게 (House Store) */
#define HOUSESTORE_POS       XMFLOAT4{5.49133f, 3.19588f, 5.35123f, 1.f}
#define HOUSESTORE_TARGETPOS XMFLOAT4{5.49133f, 4.19588f, 5.35123f, 1.f}

/* 텐트 */
#define TENT_POS             XMFLOAT4{4.01082f, -1.47512f, -11.1645f, 1.f}
#define TENT_TARGETPOS       XMFLOAT4{4.01082f, -0.47512f, -11.1645f, 1.f}

/* 신혼집 (Wedding) */
#define WEDDING_POS          XMFLOAT4{10.1104f, 0.658645f, -0.110836f, 1.f}
#define WEDDING_TARGETPOS    XMFLOAT4{10.1104f, 1.658645f, -0.110836f, 1.f}

/* 상성 테스터 (Love) */
#define LOVE_POS             XMFLOAT4{11.1245f, -2.05861f, -4.35425f, 1.f}
#define LOVE_TARGETPOS       XMFLOAT4{11.1245f, -1.05861f, -4.35425f, 1.f}

/* 항구 (Port) */
#define PORT_POS             XMFLOAT4{9.34513f, -1.42651f, -14.7418f, 1.f}
#define PORT_TARGETPOS       XMFLOAT4{9.34513f, -0.42651f, -14.7418f, 1.f}

/* 전망대 (Jeonmangdae) */
#define JEONMANGDAE_POS      XMFLOAT4{9.71453f, 3.16803f, 4.44716f, 1.f}
#define JEONMANGDAE_TARGETPOS XMFLOAT4{9.71453f, 4.16803f, 4.44716f, 1.f}

/* 분수 (Fountain) */
#define FOUNTAIN_POS         XMFLOAT4{-1.91f, -0.07f, -6.20f, 1.f}
#define FOUNTAIN_TARGETPOS   XMFLOAT4{-1.91f,  0.93f, -6.20f, 1.f}

/* 놀이공원 (Play) */
#define PLAY_POS             XMFLOAT4{26.0447f, -2.04709f, 17.6873f, 1.f}
#define PLAY_TARGETPOS       XMFLOAT4{26.0447f, -1.04709f, 17.6873f, 1.f}

/* 랭크 (Rank) */
#define RANK_POS             XMFLOAT4{4.68566f, -0.900694f, -7.67046f, 1.f}
#define RANK_TARGETPOS       XMFLOAT4{4.68566f,  0.099306f, -7.67046f, 1.f}

/* 내보내기 (Export) */
#define EXPORT_POS           XMFLOAT4{17.0699f, -2.62706f, -14.7365f, 1.f}
#define EXPORT_TARGETPOS     XMFLOAT4{17.0699f, -1.62706f, -14.7365f, 1.f}

/* 공원 (Park) */
#define PARK_POS             XMFLOAT4{19.7703f, 3.18383f, 21.9835f, 1.f}
#define PARK_TARGETPOS       XMFLOAT4{19.7703f, 4.18383f, 21.9835f, 1.f}

/* 사진관 (Photo) */
#define PHOTO_POS            XMFLOAT4{19.2285f, -2.02538f, 16.3784f, 1.f}
#define PHOTO_TARGETPOS      XMFLOAT4{19.2285f, -1.02538f, 16.3784f, 1.f}

/* 럭셔리 집/호텔 (Luxury) */
#define LUXURY_POS           XMFLOAT4{-1.36448f, -1.50474f, -12.5708f, 1.f}
#define LUXURY_TARGETPOS     XMFLOAT4{-1.36448f, -0.50474f, -12.5708f, 1.f}

/* 바다 (Sea) */
#define SEA_POS              XMFLOAT4{0.407133f, -2.21341f, -17.2402f, 1.f}
#define SEA_TARGETPOS        XMFLOAT4{0.407133f, -1.21341f, -17.2402f, 1.f}

/* 카페 (Cafe) */
#define CAFE_POS             XMFLOAT4{22.5624f, -2.02538f, 15.4615f, 1.f}
#define CAFE_TARGETPOS       XMFLOAT4{22.5624f, -1.02538f, 15.4615f, 1.f}

CPlace_Island::CPlace_Island(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Island::Initialize()
{
	if (FAILED(Ready_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Camera_Layer()))
		return E_FAIL;

	m_tmpTargetPos = XMVECTOR{ 10.f, 10.f, 0.f, 1.f };
	return S_OK;
}

int32_t CPlace_Island::Update(_float fDT)
{
	double fps = 1.f/ fDT;
	WCHAR buf[128];
	swprintf_s(buf, L"FPS: %.1f", fps);   // 소수점 1자리
	SetWindowTextW(g_hWnd, buf);

	return 0;
}

HRESULT CPlace_Island::Render_Priority()
{
	return S_OK;
}

HRESULT CPlace_Island::Render_Late()
{
	return S_OK;
}

HRESULT CPlace_Island::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ISLAND);


	// ProtoTag_Actor_Island
	{
		CIsland::ISLAND_DESC tIslandDesc = {};
		tIslandDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f,1.f };
		tIslandDesc.fCX = 1.f;
		tIslandDesc.fCY = 1.f;
		tIslandDesc.fCZ = 1.f;
		tIslandDesc.fSpeedPerSecond = 0.f;
		tIslandDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tIslandDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;


		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::ISLAND),
			TEXT("ProtoTag_Actor_Island"),
			&tIslandDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::ISLAND),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}


	// ProtoTag_Actor_Sky
	{
		CSky_Actor::SKY_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		Desc.fCX = 1.f;
		Desc.fCY = 1.f;
		Desc.fCZ = 1.f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND),TEXT("ProtoTag_Actor_Sky"),&Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND),TEXT("LayerTag_Actor"),tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_Actor_Sea
	{
		CSea_Actor::SEA_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		Desc.fCX = 1.f;
		Desc.fCY = 1.f;
		Desc.fCZ = 1.f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Actor_Sea"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}
	
	CTempModel::TEMPMODEL_DESC tTmpModelDesc = {};

	XMFLOAT4 vPlacePosArr[] = {
		// 0. Go Apart
		APART_POS,

		// 1. Go Wear
		WEARSTORE_POS,

		// 2. Go Antique
		ANTIQUE_POS,

		// 3. Go Cap
		CAPSTORE_POS,

		// 4. Go Food
		FOODSTORE_POS,

		// 5. Go Luxury
		LUXURY_POS,

		// 6. Go Music
		MUSICHALL_POS,

		// 7. Go House
		HOUSESTORE_POS,

		// 8. Go CityHall
		CITYHALL_POS,

		// 9. Go Rank
		RANK_POS,

		// 10. Go Tent
		TENT_POS,

		// 11. Go wedding
		WEDDING_POS,

		// 12. Go love
		LOVE_POS,

		// 13. Go port
		PORT_POS,

		// 14. Go photo
		PHOTO_POS,

		// 15. Go news
		NEWSSTATION_POS,

		// 16. Go export
		EXPORT_POS,

		// 17. Go Fountain
		FOUNTAIN_POS,

		// 18. Go Sea
		SEA_POS,

		// 19. Go jeonMangDae
		JEONMANGDAE_POS,

		// 20. Go park
		PARK_POS,

		// 21. Go play
		PLAY_POS,

		// 22. Go cafe
		CAFE_POS
	};

	uint32_t BuildingIndexArr[] = {
		// 0. Go Apart
		1,
		// 1. Go Wear
		2,
		// 2. Go Antique
		3,
		// 3. Go Cap
		4,
		// 4. Go Food
		5,
		// 5. Go Luxury
		7, 
		// 6. Go Music
		8, 
		// 7. Go House
		10,
		// 8. Go CityHall
		11,
		// 9. Go Rank
		12,
		// 10. Go Tent
		13,
		// 11. Go wedding
		15,
		// 12. Go love
		16,
		// 13. Go port
		17,
		// 14. Go photo
		19,
		// 15. Go news
		20,
		// 16. Go export
		23,
		// 17. Go Fountain
		6, 
		// 18. Go Sea
		9,
		// 19. Go jeonMangDae
		14,
		// 20. Go park
		18,
		// 21. Go play
		21,
		// 22. Go cafe
		22
	};

	constexpr uint32_t iNumPlace = sizeof(vPlacePosArr) / sizeof(XMFLOAT4);

	IUnlock* pUnlockStrgyArr[] = {
		// 0. Go Apart
		CApart_UnlockStrategy::Create(),
		// 1. Go Wear
		CClothStore_UnlockStrategy::Create(),
		// 2. Go Antique
		COtherBuilding_UnlockStrategy::Create(),
		// 3. Go Cap
		COtherBuilding_UnlockStrategy::Create(),
		// 4. Go Food
		CFoodStore_UnlockStrategy::Create(),
		// 5. Go Luxury
		COtherBuilding_UnlockStrategy::Create(),
		// 6. Go Music
		COtherBuilding_UnlockStrategy::Create(),
		// 7. Go House
		CHouse_UnlockStrategy::Create(),
		// 8. Go CityHall
		CCityHall_UnlockStrategy::Create(),
		// 9. Go Rank
		COtherBuilding_UnlockStrategy::Create(),
		// 10. Go Tent
		COtherBuilding_UnlockStrategy::Create(),	
		// 11. Go wedding
		COtherBuilding_UnlockStrategy::Create(),	
		// 12. Go love
		COtherBuilding_UnlockStrategy::Create(),	
		// 13. Go port
		COtherBuilding_UnlockStrategy::Create(),	
		// 14. Go photo
		COtherBuilding_UnlockStrategy::Create(),	
		// 15. Go news
		COtherBuilding_UnlockStrategy::Create(),	
		// 16. Go export
		COtherBuilding_UnlockStrategy::Create(),	
		// 17. Go Fountain
		COtherBuilding_UnlockStrategy::Create(),	
		// 18. Go Sea
		COtherBuilding_UnlockStrategy::Create(),	
		// 19. Go jeonMangDae
		COtherBuilding_UnlockStrategy::Create(),	
		// 20. Go park
		COtherBuilding_UnlockStrategy::Create(),	
		// 21. Go play
		COtherBuilding_UnlockStrategy::Create(),	
		// 22. Go cafe
		COtherBuilding_UnlockStrategy::Create(), 
	};

	/* 건물들 */
	for (uint32_t i = 0; i < iNumPlace; ++i)
	{
		tTmpModelDesc.vPos = vPlacePosArr[i];
		tTmpModelDesc.fCX = 1.f;
		tTmpModelDesc.fCY = 1.f;
		tTmpModelDesc.fCZ = 1.f;
		tTmpModelDesc.iBuildingIndex = BuildingIndexArr[i];
		tTmpModelDesc.fSpeedPerSecond = 3.f;
		tTmpModelDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		tTmpModelDesc.fRotationPerSecond = XMConvertToRadians(0.f);
		tTmpModelDesc.pUnlockStrategy = pUnlockStrgyArr[i];

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Actor_TempModel"), &tTmpModelDesc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	
	return S_OK;
}

HRESULT CPlace_Island::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ISLAND);

	/* 위치 배열 */
	const XMFLOAT3 vPosArr[] =	{
		// 0. Go Apart
		XMFLOAT3{385.f, 135.f, 0.4f},

		// 1. Go Wear
		XMFLOAT3{270.f,  70.f, 0.4f},

		// 2. Go Antique
		XMFLOAT3{220.f, 150.f, 0.4f},

		// 3. Go Cap
		XMFLOAT3{225.f, 250.f, 0.4f},

		// 4. Go Food
		XMFLOAT3{210.f, 340.f, 0.4f},

		// 5. Go Luxury
		XMFLOAT3{300.f, 340.f, 0.4f},

		// 6. Go Music
		XMFLOAT3{400.f, 250.f, 0.4f},

		// 7. Go House
		XMFLOAT3{490.f,  60.f, 0.4f},

		// 8. Go CityHall
		XMFLOAT3{500.f, 150.f, 0.4f},

		// 9. Go Rank
		XMFLOAT3{475.f, 250.f, 0.4f},

		// 10. Go Tent
		XMFLOAT3{475.f, 340.f, 0.4f},

		// 11. Go wedding
		XMFLOAT3{600.f, 150.f, 0.4f},

		// 12. Go love
		XMFLOAT3{570.f, 260.f, 0.4f},

		// 13. Go port
		XMFLOAT3{600.f, 375.f, 0.4f},

		// 14. Go photo
		XMFLOAT3{675.f, 250.f, 0.4f},

		// 15. Go news
		XMFLOAT3{740.f, 90.f, 0.4f},

		// 16. Go export
		XMFLOAT3{740.f, 370.f, 0.4f},

		// 17. Go Fountain
		XMFLOAT3{310.f, 250.f, 0.4f},

		// 18. Go Sea
		XMFLOAT3{400.f, 373.f, 0.4f},

		// 19. Go jeonMangDae
		XMFLOAT3{565.f,  60.f, 0.4f},

		// 20. Go park
		XMFLOAT3{660.f,  60.f, 0.4f},

		// 21. Go play
		XMFLOAT3{760.f, 175.f, 0.4f},

		// 22. Go cafe
		XMFLOAT3{760.f, 260.f, 0.4f},
	};

	/* 텍스쳐 인덱스 배열 */
	const POINT TextureIndexArr[] =	{
		{44, 4},  // 0. Go Apart
		{ 0, 0},  // 1. Go Wear
		{ 4, 0},  // 2. Go Antique
		{ 6, 0},  // 3. Go Cap
		{16, 0},  // 4. Go Food
		{18, 0},  // 5. Go Luxury
		{ 8, 0},  // 6. Go Music
		{ 2, 0},  // 7. Go House
		{38, 2},  // 8. Go CityHall
		{10, 0},  // 9. Go Rank  
		{20, 0},  // 10. Go Tent
		{40, 0},  // 11. Go Wedding
		{12, 0},  // 12. Go Love
		{22, 0},  // 13. Go Port
		{14, 0},  // 14. Go Photo
		{42, 3},  // 15. Go News
		{24, 0},  // 16. Go Export 
		{32, 1},  // 17. Go Fountain
		{34, 1},  // 18. Go Sea
		{26, 1},  // 19. Go JeonMangDae
		{28, 1},  // 20. Go Park
		{30, 1},  // 21. Go Play
		{36, 1},  // 22. Go Cafe
	};

	/* 타겟 위치 */
	const XMFLOAT4 vTargetPosArr[] =	{
		// 0. Go Apart
		APART_TARGETPOS,

		// 1. Go Wear
		WEARSTORE_TARGETPOS,

		// 2. Go Antique
		ANTIQUE_TARGETPOS,

		// 3. Go Cap
		CAPSTORE_TARGETPOS,

		// 4. Go Food
		FOODSTORE_TARGETPOS,

		// 5. Go Luxury
		LUXURY_TARGETPOS,

		// 6. Go Music
		MUSICHALL_TARGETPOS,

		// 7. Go House
		HOUSESTORE_TARGETPOS,

		// 8. Go CityHall
		CITYHALL_TARGETPOS,

		// 9. Go Rank
		RANK_TARGETPOS,

		// 10. Go Tent
		TENT_TARGETPOS,

		// 11. Go wedding
		WEDDING_TARGETPOS,

		// 12. Go love
		LOVE_TARGETPOS,

		// 13. Go port
		PORT_TARGETPOS,

		// 14. Go photo
		PHOTO_TARGETPOS,

		// 15. Go news
		NEWSSTATION_TARGETPOS,

		// 16. Go export
		EXPORT_TARGETPOS,

		// 17. Go Fountain
		FOUNTAIN_TARGETPOS,

		// 18. Go Sea
		SEA_TARGETPOS,

		// 19. Go jeonMangDae
		JEONMANGDAE_TARGETPOS,

		// 20. Go park
		PARK_TARGETPOS,

		// 21. Go play
		PLAY_TARGETPOS,

		// 22. Go cafe
		CAFE_TARGETPOS,
	};

	const uint32_t BuildingIndexArr[] = {
		1,	// 0. Go Apart
		2,	// 1. Go Wear
		3,	// 2. Go Antique
		4,	// 3. Go Cap
		5,	// 4. Go Food
		7,	// 5. Go Luxury
		8,	// 6. Go Music
		10,	// 7. Go House
		11,	// 8. Go CityHall
		12,	// 9. Go Rank   
		13,	// 10. Go Tent
		15,	// 11. Go Wedding
		16,	// 12. Go Love
		17,	// 13. Go Port
		19,	// 14. Go Photo
		20,	// 15. Go News
		23,	// 16. Go Export 
		6,	// 17. Go Fountain
		9,	// 18. Go Sea
		14,	// 19. Go JeonMangDae
		18,	// 20. Go Park
		21,	// 21. Go Play
		22	// 22. Go Cafe
	};

	{

		PLACEID NextPlaceIDArr[] = {
		PLACEID::APART,	// 0. Go Apart
		PLACEID::CLOTH_STORE,// 1. Go Wear
		PLACEID::APART,// 2. Go Antique
		PLACEID::APART,// 3. Go Cap
		PLACEID::FOOD_STORE, // 4. Go Food
		PLACEID::APART, // 5. Go Luxury
		PLACEID::APART, // 6. Go Music
		PLACEID::APART,  // 7. Go House
		PLACEID::CITYHALL,  // 8. Go CityHall
		PLACEID::APART,// 9. Go Rank   
		PLACEID::APART,// 10. Go Tent
		PLACEID::APART,// 11. Go Wedding
		PLACEID::APART,// 12. Go Love
		PLACEID::APART,// 13. Go Port
		PLACEID::APART,// 14. Go Photo
		PLACEID::APART,// 15. Go News
		PLACEID::DEVELOPER,// 16. Go Export 
		PLACEID::APART,// 17. Go Fountain
		PLACEID::APART,// 18. Go Sea
		PLACEID::APART,// 19. Go JeonMangDae
		PLACEID::APART,// 20. Go Park
		PLACEID::APART,// 21. Go Play
		PLACEID::APART	 // 22. Go Cafe
		};
		CPlaceMoveButton::PLACEMOVEBUTTON_DESC PM_Desc = {};
		constexpr uint32_t iNumPlace = sizeof(vPosArr) / sizeof(vPosArr[0]);
		_bool isApart = { FALSE };

		/* LayerTag_UI_PlaceMoveButton */
		for (uint32_t i = 0; i < iNumPlace; ++i)
		{
			PM_Desc.fX = vPosArr[i].x;
			PM_Desc.fY = vPosArr[i].y;
			PM_Desc.fZ = vPosArr[i].z;

			if (isApart == FALSE)
			{
				PM_Desc.fCX = 130.f;
				PM_Desc.fCY = 120.f;
				isApart = TRUE;
			}
			else
			{
				PM_Desc.fCX = 115.f;
				PM_Desc.fCY = 115.f;
			}

			PM_Desc.fSpeedPerSecond = 0.f;
			PM_Desc.eCurPlaceID = PLACEID::ISLAND;
			PM_Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			PM_Desc.fRotationPerSecond = XMConvertToRadians(20.f);
			PM_Desc.iTextureIndex = static_cast<uint32_t>(TextureIndexArr[i].x);
			PM_Desc.iOutlineTextureIndex = static_cast<uint32_t>(TextureIndexArr[i].y);

			/* 전략 */
			CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgy_Desc = {};
			strgy_Desc.ePlaceID		= PLACEID::ISLAND;
			strgy_Desc.strLayerTag	= TEXT("LayerTag_UI_PlaceMoveButton");
			strgy_Desc.EnableLayers = vector<_wstring>{ TEXT("LayerTag_UI_Back_Button") };
			strgy_Desc.DisableLayers = vector<_wstring>{TEXT("LayerTag_UI_Wallet")};

			PM_Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext,strgy_Desc);

			CCameraZoom_Strgy::CAMERAZOOMSTRGY_DESC tCameraZoomStrgyDesc = {};
			tCameraZoomStrgyDesc.strCameraTag	= TEXT("World_Camera");
			tCameraZoomStrgyDesc.fTargetFov		= XMConvertToRadians(10.f);
			tCameraZoomStrgyDesc.vTargetPos		= vTargetPosArr[i];
			tCameraZoomStrgyDesc.fLerpTime		= 0.1f;
			PM_Desc.pSelected_Strgy = CCameraZoom_Strgy::Create(m_pDevice, m_pContext, tCameraZoomStrgyDesc);

			PM_Desc.pSelectedClick_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, NextPlaceIDArr[i]);
			PM_Desc.iMatchBuildingIndex = BuildingIndexArr[i];
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_UI_PlaceMoveButton"), &PM_Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_PlaceMoveButton"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_PlaceMoveButton"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}
	}

	// ProtoTag_UI_BuildingName_Texture
	{
		CBuildingName_Texture::BUILDINGNAME_DESC tBuildingNameDesc = {};
		tBuildingNameDesc.eRenderGroup = RENDER_GROUP::TOP_UI_BLEND;
		tBuildingNameDesc.fX = 480.f;
		tBuildingNameDesc.fY = 60.f;
		tBuildingNameDesc.fZ = 0.5f;
		tBuildingNameDesc.fCX = 400.f;
		tBuildingNameDesc.fCY = 100.f;
		tBuildingNameDesc.fSpeedPerSecond = 10.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_UI_BuildingNameTexture"), &tBuildingNameDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_BuildingName_Texture"), tmp)))
			return E_FAIL;

		// if (FAILED(m_pGameInstance->ButtonManager_Add_Button(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_BuildingName_Texture"), static_cast<CButton*>(tmp))))
		// 	return E_FAIL;
	}


	 // ProtoTag_UI_BackGround
	 {
		 CBackGround::BACKGROUND_DESC tBackGroundDesc = {};
		 tBackGroundDesc.fX = 480.f;
		 tBackGroundDesc.fY = 240.f;
		 tBackGroundDesc.fZ = 1.f;
		 tBackGroundDesc.fCX = 960.f;
		 tBackGroundDesc.fCY = 480.f;
		 tBackGroundDesc.fSpeedPerSecond = 0.f;
		 tBackGroundDesc.fRotationPerSecond = XMConvertToRadians(0.f);
		 tBackGroundDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		 tBackGroundDesc.eTexturePlaceID = PLACEID::ISLAND;
		 tBackGroundDesc.strTextureTag = TEXT("ProtoTag_Component_BackGround_Texture");

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_BackGround"),
			 &tBackGroundDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	 /* 세이브 버튼 */
	 // ProtoTag_UI_Save_Button
	 {
		 CSave_Button::SAVEBUTTON_DESC Desc = {};
		 Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		 Desc.fCX = 189.f;
		 Desc.fCY = 99.f;
		 Desc.fX = 960.f - Desc.fCX * 0.5f + 4.f;
		 Desc.fY = 480.f - Desc.fCY * 0.5f + 4.f;
		 Desc.fZ = 0.5f;

		 CIconTexture::ICONTEXTURE_DESC IconDesc = {};
		 IconDesc.eRenderGroup = Desc.eRenderGroup;
		 IconDesc.fZ = 0.4f;

		 Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext,PLACEID::ISLAND, TEXT("LayerTag_UI_Save_Button"));
		 Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		 Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Save_Button"), &Desc);
		 if (nullptr == tmp)
			 return E_FAIL;

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Save_Button"), tmp)))
			 return E_FAIL;

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Save_Button"), static_cast<CButton*>(tmp))))
			 return E_FAIL;

		 /* 아이콘 텍스쳐 */
		 IconDesc.fX = Desc.fX;
		 IconDesc.fY = Desc.fY - 4.f;
		 IconDesc.iTextureIndex = 22;
		 IconDesc.fCX = 96.f;
		 IconDesc.fCY = 30.f;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Icon"), &IconDesc);
		 if (nullptr == tmp)
			 return E_FAIL;

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Save_Button"), tmp)))
			 return E_FAIL;
	 }

	 /* 지갑 */
	 // ProtoTag_UI_Wallet
	 {
		 CWallet::WALLET_DESC Desc = {};
		 Desc.fCX = 320.f;
		 Desc.fCY = 48.f;
		 Desc.fX = 0.f + Desc.fCX * 0.5f + 10.f;
		 Desc.fY = 480.f - Desc.fCY * 0.5f - 5.f;
		 Desc.fZ = 0.5f;
		 Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		 CCameraZoom_Strgy::CAMERAZOOMSTRGY_DESC strgy_Desc = {};
		 strgy_Desc.strCameraTag = TEXT("World_Camera");
		 strgy_Desc.fTargetFov = XMConvertToRadians(45.f);
		 strgy_Desc.vTargetPos = XMFLOAT4{ 2.f, 0.f, 0.f, 1.f };
		 strgy_Desc.fLerpTime = 0.1f;

		 Desc.pUpdateStrgy = CCameraZoom_Strgy::Create(m_pDevice, m_pContext, strgy_Desc);

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Wallet"), &Desc);
		 if (nullptr == tmp)
			 return E_FAIL;

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Wallet"), tmp)))
			 return E_FAIL;
	 }
	 

	 /* 뒤로가기 버튼 */
	// LayerTag_UI_Back_Button
	 {
		 CBack_Button::BACKBUTTON_DESC Desc = {};
		 Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		 Desc.fCX = 100.f;
		 Desc.fCY = 68.f;
		 Desc.fX = 50.f;
		 Desc.fY = 446.f;
		 Desc.fZ = 0.5f;

		 CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgy_Desc = {};
		 strgy_Desc.ePlaceID = PLACEID::ISLAND;
		 strgy_Desc.strLayerTag = TEXT("LayerTag_UI_PlaceMoveButton");
		 strgy_Desc.EnableLayers = vector<_wstring>{ TEXT("LayerTag_UI_Wallet") };
		 strgy_Desc.DisableLayers = vector<_wstring>{ TEXT("LayerTag_UI_Back_Button") };
		 
		 Desc.pClicked_Strgy = CControlEnable_Strgy::Create	(m_pDevice, m_pContext, strgy_Desc);
		 Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		 Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Back_Button"), &Desc);
		 if (nullptr == tmp)
			 return E_FAIL;

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Back_Button"), tmp)))
			 return E_FAIL;

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Back_Button"), static_cast<CButton*>(tmp))))
			 return E_FAIL;
	 }


	 if (FAILED(Ready_Layer_Windows()))
		 return E_FAIL;

	 m_pGameInstance->ObjectManager_Enable_Layer(iPlaceID, TEXT("LayerTag_UI_Wallet"));
	 m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, TEXT("LayerTag_UI_Back_Button"));
	return S_OK;
}

HRESULT CPlace_Island::Ready_Environment_Layer()
{
	LIGHT_DESC Desc = {};
	Desc.eType = LIGHT_TYPE::DIRECTIONAL;
	Desc.vDir = _float4(1.f, -1.f, 1.f, 0.f);
	Desc.vDiffuse = _float4(0.4f, 0.4f, 0.4f, 1.f);
	Desc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	Desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

	Desc.eType = LIGHT_TYPE::POINT;
	Desc.vPos = _float4(0.f, 500.f, -30.f, 1.f);
	Desc.fRange = 1000.f;
	Desc.vDiffuse = _float4(1.f, 0.3f, 0.3f, 1.f);
	Desc.vAmbient = _float4(0.3f, 0.1f, 0.1f, 1.f);
	Desc.vSpecular = Desc.vDiffuse;

	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;


	Desc.eType = LIGHT_TYPE::POINT;
	Desc.vPos = _float4(0.f, 500.f, 30.f, 1.f);
	Desc.fRange = 1000.f;
	Desc.vDiffuse = _float4(0.3f, 0.3f, 1.f, 1.f);
	Desc.vAmbient = _float4(0.1f, 0.1f, 0.3f, 1.f);
	Desc.vSpecular = Desc.vDiffuse;

	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlace_Island::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 2.f, 5.f, -30.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 2.f, 0.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);

	return S_OK;
}

HRESULT CPlace_Island::Ready_Layer_Windows()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ISLAND);


	// ProtoTag_UI_WindowButton
	{
		CWindow_Button::WINDOWBUTTON_DESC tDesc = {};
		tDesc.fCX = 0.5f;
		tDesc.fCY = 0.5f;
		tDesc.fZ = -2.5f;
		tDesc.strCameraTag = TEXT("World_Camera");

		uint32_t iIndex = { 0 };

		/* 주민들의 이름 */

		vector<_wstring > Layers;
		uint32_t iNumMii = CApartManager::GetInstance()->Get_ApartInfo_NumMii();
		Layers.reserve(iNumMii);

		for (uint32_t i = 0; i < iNumMii; ++i)
		{
			Layers.push_back(CApartManager::GetInstance()->Get_ApartInfo_MiiName(i));
		}

		for (int32_t i = 2; i >= 0; --i)
		{
			for (int32_t j = 0; j < 8; ++j)
			{
				tDesc.eRenderGroup = RENDER_GROUP::TOP_UI;
				tDesc.fX = 478.3f + (0.5f * j);
				tDesc.fY = 237.5f + (0.5f * i);

				tDesc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				/* 인덱스로 보관 */

				if (CApartManager::GetInstance()->Get_ApartInfo_NumMii() > iIndex)
				{
					tDesc.iMiiIndex = iIndex; 
					tDesc.isLive = TRUE;

				}
				else
				{
					tDesc.iMiiIndex = 0;
					tDesc.isLive = FALSE;
				}

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_WindowButton"), &tDesc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_WindowButton"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_WindowButton"), dynamic_cast<CButton*>(tmp))))
					return E_FAIL;

				++iIndex;
			}
		}
	}
	return S_OK;
}


CPlace_Island* CPlace_Island::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Island* pPlaceMain = new CPlace_Island(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Island Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Island::Free()
{
	CPlace::Free();
}
