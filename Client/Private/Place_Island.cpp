#include "Place_Island.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "Island.h"
#include "SkyAndSea.h"
#include "PlaceMoveButton.h"
#include "BackGround.h"
#include "Apart.h"
#include "TempModel.h"
#include "ChangePlace_Strategy.h"
#include "CameraZoom_Strgy.h"
#include "Deactive_Strgy.h"

#include "IUnlock.h"
#include "LoadingBG.h"
#include "BuildingName_Texture.h"


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

	// ProtoTag_Actor_SkyAndSea
	{
		CSkyAndSea::SKYANDSEA_DESC tSkyAndSeaDesc = {};
		tSkyAndSeaDesc.vPos = XMFLOAT4{0.f, 0.f, 0.f,1.f};
		tSkyAndSeaDesc.fCX = 1.f;
		tSkyAndSeaDesc.fCY = 1.f;
		tSkyAndSeaDesc.fCZ = 1.f;
		tSkyAndSeaDesc.fSpeedPerSecond = 0.f;
		tSkyAndSeaDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tSkyAndSeaDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::ISLAND),
			TEXT("ProtoTag_Actor_SkyAndSea"),
			&tSkyAndSeaDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::ISLAND),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	//// ProtoTag_Actor_Apart
	//{
	//	CApart::APART_DESC tApartDesc = {};
	//	tApartDesc.fX = 0.f;
	//	tApartDesc.fY = 0.f;
	//	tApartDesc.fZ = 0.f;
	//	tApartDesc.fCX = 1.f;
	//	tApartDesc.fCY = 1.f;
	//	tApartDesc.fCZ = 1.f;
	//	tApartDesc.fSpeedPerSecond = 0.f;
	//	tApartDesc.fRotationPerSecond = XMConvertToRadians(20.f);
	//	tApartDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

	//	tmp = m_pGameInstance->Proto_Get_Object_Clone(
	//		static_cast<uint32_t>(PLACEID::ISLAND),
	//		TEXT("ProtoTag_Actor_Apart"),
	//		&tApartDesc);

	//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
	//		static_cast<uint32_t>(PLACEID::ISLAND),
	//		TEXT("LayerTag_Actor"),
	//		tmp)))
	//	{
	//		Safe_Release(tmp);
	//		return E_FAIL;
	//	}
	//}

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
		tTmpModelDesc.pUnlockStrategy = CFoodStore_UnlockStrategy::Create();

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


	/* 위치 배열 */
	const XMFLOAT3 vPosArr[] =
	{
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
	const POINT TextureIndexArr[] =
	{
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
	const XMFLOAT4 vTargetPosArr[] =
	{
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
		5,   // 4. Go Food
		7,   // 5. Go Luxury
		8,   // 6. Go Music
		10,   // 7. Go House
		11,   // 8. Go CityHall
		12,   // 9. Go Rank   
		13,   // 10. Go Tent
		15,   // 11. Go Wedding
		16,   // 12. Go Love
		17,   // 13. Go Port
		19,   // 14. Go Photo
		20,   // 15. Go News
		23,   // 16. Go Export 
		6,   // 17. Go Fountain
		9,   // 18. Go Sea
		14,   // 19. Go JeonMangDae
		18,   // 20. Go Park
		21,   // 21. Go Play
		22   // 22. Go Cafe
	};

	PLACEID NextPlaceIDArr[] = {
	PLACEID::APART,	// 0. Go Apart
	PLACEID::APART,// 1. Go Wear
	PLACEID::APART,// 2. Go Antique
	PLACEID::APART,// 3. Go Cap
	PLACEID::APART, // 4. Go Food
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
	CPlaceMoveButton::PLACEMOVEBUTTON_DESC tPlaceMoveButtonDesc = {};
	constexpr uint32_t iNumPlace = sizeof(vPosArr) / sizeof(vPosArr[0]);
	_bool isApart = { FALSE };

	/* LayerTag_UI_PlaceMoveButton */
	for (uint32_t i = 0; i < iNumPlace; ++i)
	{
		tPlaceMoveButtonDesc.fX = vPosArr[i].x;
		tPlaceMoveButtonDesc.fY = vPosArr[i].y;
		tPlaceMoveButtonDesc.fZ = vPosArr[i].z;

		if (isApart == FALSE)
		{
			tPlaceMoveButtonDesc.fCX = 130.f;
			tPlaceMoveButtonDesc.fCY = 120.f;
			isApart = TRUE;
		}
		else
		{
			tPlaceMoveButtonDesc.fCX = 115.f;
			tPlaceMoveButtonDesc.fCY = 115.f;
		}

		tPlaceMoveButtonDesc.fSpeedPerSecond = 0.f;
		tPlaceMoveButtonDesc.eCurPlaceID = PLACEID::ISLAND;
		tPlaceMoveButtonDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		tPlaceMoveButtonDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tPlaceMoveButtonDesc.iTextureIndex = static_cast<uint32_t>(TextureIndexArr[i].x);
		tPlaceMoveButtonDesc.iOutlineTextureIndex = static_cast<uint32_t>(TextureIndexArr[i].y);

		/* 전략 */
		tPlaceMoveButtonDesc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ISLAND, TEXT("LayerTag_UI_PlaceMoveButton"));

		CCameraZoom_Strgy::CAMERAZOOMSTRGY_DESC tCameraZoomStrgyDesc = {};
		tCameraZoomStrgyDesc.strCameraTag = TEXT("World_Camera");
		tCameraZoomStrgyDesc.fTargetFov = XMConvertToRadians(10.f);
		tCameraZoomStrgyDesc.vTargetPos = vTargetPosArr[i];
		tCameraZoomStrgyDesc.fLerpTime = 0.1f;
		tPlaceMoveButtonDesc.pSelected_Strgy = CCameraZoom_Strgy::Create(m_pDevice, m_pContext, &tCameraZoomStrgyDesc);

		tPlaceMoveButtonDesc.pSelectedClick_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, NextPlaceIDArr[i]);
		tPlaceMoveButtonDesc.iMatchBuildingIndex = BuildingIndexArr[i];
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_UI_PlaceMoveButton"), &tPlaceMoveButtonDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_PlaceMoveButton"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_PlaceMoveButton"), static_cast<CButton*>(tmp))))
			return E_FAIL;
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

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI_BuildingName_Texture"), static_cast<CButton*>(tmp))))
			return E_FAIL;
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
	 
	return S_OK;
}

HRESULT CPlace_Island::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Island::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 2.f, 5.f, -30.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 2.f, 0.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);

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
