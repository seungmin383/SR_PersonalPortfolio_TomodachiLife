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
#include "IUnlock.h"
#include "LoadingBG.h"

/* y += 1.f */
#define FOODSTORE_POS XMFLOAT4{- 7.47224f,-0.561126f, -10.7086f, 1.f}
#define FOODSTORE_TARGETPOS XMFLOAT4{- 7.47224f,0.561126f, -10.7086f, 1.f}

/* y += 1.9f */
#define APART_POS XMFLOAT4{0.f, 0.3f, 0.f, 1.f}
#define APART_TARGETPOS XMFLOAT4{0.f, 2.2f, 0.f, 1.f}

/* y += 2.f */
#define CITYHALL_POS XMFLOAT4{4.93322f, 0.0844781f, -2.84163f, 1.f}
#define CITYHALL_TARGETPOS XMFLOAT4{4.93322f, 2.0844781f, -2.84163f, 1.f}

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

	
		//m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), m_tmpTargetPos,
		//	XMConvertToRadians(30.f), fDT / 3.f);

	return 0;
}

HRESULT CPlace_Island::Render_Priority()
{
	// SetWindowText(g_hWnd, TEXT("섬이다."));
	return S_OK;
}

HRESULT CPlace_Island::Render_Late()
{
	// SetWindowText(g_hWnd, TEXT("섬이다."));
	return S_OK;
}

HRESULT CPlace_Island::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	// ProtoTag_Actor_Island
	{
		CIsland::ISLAND_DESC tIslandDesc = {};
		tIslandDesc.fX = 0.f;
		tIslandDesc.fY = 0.f;
		tIslandDesc.fZ = 0.f;
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
		tSkyAndSeaDesc.fX = 0.f;
		tSkyAndSeaDesc.fY = 0.f;
		tSkyAndSeaDesc.fZ = 0.f;
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

	/* 음식점 */
	// ProtoTag_Actor_TempModel
	{
		tTmpModelDesc.fX = -7.47224f;
		tTmpModelDesc.fY = -0.561126f;
		tTmpModelDesc.fZ = -10.7086f;
		tTmpModelDesc.vPos = FOODSTORE_POS;
		tTmpModelDesc.fCX = 1.f;
		tTmpModelDesc.fCY = 1.f;
		tTmpModelDesc.fCZ = 1.f;
		tTmpModelDesc.iBuildingIndex = 4;
		tTmpModelDesc.fSpeedPerSecond = 1.f;
		tTmpModelDesc.eCurPlaceID = PLACEID::ISLAND;
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

	/* 아파트 */
	// ProtoTag_Actor_TempModel
	{
		tTmpModelDesc.fX = 0.f;
		tTmpModelDesc.fY = 0.3f;
		tTmpModelDesc.fZ = 0.f;
		tTmpModelDesc.vPos = APART_POS;
		tTmpModelDesc.iBuildingIndex = 1;
		tTmpModelDesc.pUnlockStrategy = CFoodStore_UnlockStrategy::Create();

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Actor_TempModel"), &tTmpModelDesc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	/* 주민센터 */
	// ProtoTag_Actor_TempModel
	{
		tTmpModelDesc.fX = 0.f;
		tTmpModelDesc.fY = 0.3f;
		tTmpModelDesc.fZ = 0.f;
		tTmpModelDesc.vPos = CITYHALL_POS;
		tTmpModelDesc.iBuildingIndex = 10;
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

	CPlaceMoveButton::PLACEMOVEBUTTON_DESC tPlaceMoveButtonDesc = {};

	// Go Apart
	 {
	 	tPlaceMoveButtonDesc.fX = 385.f;
	 	tPlaceMoveButtonDesc.fY = 135.f;
	 	tPlaceMoveButtonDesc.fCX = 130.f;
	 	tPlaceMoveButtonDesc.fCY = 120.f;
	 	tPlaceMoveButtonDesc.fSpeedPerSecond = 0.f;
	 	tPlaceMoveButtonDesc.fRotationPerSecond = XMConvertToRadians(20.f);
	 	tPlaceMoveButtonDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		tPlaceMoveButtonDesc.eCurPlaceID = PLACEID::ISLAND;
		tPlaceMoveButtonDesc.iTextureNum = 13;
		tPlaceMoveButtonDesc.iOutlineTextureNum = 0;
		tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);
		tPlaceMoveButtonDesc.fLerpTime = 0.1f;
		tPlaceMoveButtonDesc.fTargetFov = XMConvertToRadians(10.f);
		tPlaceMoveButtonDesc.vTargetPos = APART_TARGETPOS;
		
		tmp = m_pGameInstance->Proto_Get_Object_Clone(
	 		static_cast<uint32_t>(PLACEID::ISLAND),
	 		TEXT("ProtoTag_UI_PlaceMoveButton"),
	 		&tPlaceMoveButtonDesc);
	 
	 	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
	 		static_cast<uint32_t>(PLACEID::ISLAND),
	 		TEXT("LayerTag_UI"),
	 		tmp)))
	 	{
	 		Safe_Release(tmp);
	 		return E_FAIL;
	 	}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}
	 }


	 // Go Wear
	 {
		 tPlaceMoveButtonDesc.fX = 270.f;
		 tPlaceMoveButtonDesc.fY = 70.f;
		 tPlaceMoveButtonDesc.fCX = 115.f;
		 tPlaceMoveButtonDesc.fCY = 115.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 0;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // Go Antique
	 {
		 tPlaceMoveButtonDesc.fX = 220.f;
		 tPlaceMoveButtonDesc.fY = 150.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 2;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }


	 // Go Cap
	 {
		 tPlaceMoveButtonDesc.fX = 225.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 3;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go Food
	 {
		 tPlaceMoveButtonDesc.fX = 210.f;
		 tPlaceMoveButtonDesc.fY = 340.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 8;
		 tPlaceMoveButtonDesc.vTargetPos = FOODSTORE_TARGETPOS;
		
		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go Luxury
	 {
		 tPlaceMoveButtonDesc.fX = 300.f;
		 tPlaceMoveButtonDesc.fY = 340.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 9;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go Music
	 {
		 tPlaceMoveButtonDesc.fX = 400.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 4;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	

	// Go House
	 {
		 tPlaceMoveButtonDesc.fX = 490.f;
		 tPlaceMoveButtonDesc.fY = 60.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 1;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go CityHall
	 {
		 tPlaceMoveButtonDesc.fX = 500.f;
		 tPlaceMoveButtonDesc.fY = 150.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::CITYHALL);

		 tPlaceMoveButtonDesc.iTextureNum = 16;
		 tPlaceMoveButtonDesc.vTargetPos = CITYHALL_TARGETPOS;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go Rank
	 {
		 tPlaceMoveButtonDesc.fX = 475.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 5;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go Tent
	 {
		 tPlaceMoveButtonDesc.fX = 475.f;
		 tPlaceMoveButtonDesc.fY = 340.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 10;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }


	// Go wedding
	 {
		 tPlaceMoveButtonDesc.fX = 600.f;
		 tPlaceMoveButtonDesc.fY = 150.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 15;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go love
	 {
		 tPlaceMoveButtonDesc.fX = 570.f;
		 tPlaceMoveButtonDesc.fY = 260.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 6;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go port
	 {
		 tPlaceMoveButtonDesc.fX = 600.f;
		 tPlaceMoveButtonDesc.fY = 375.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 11;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 

	// Go photo
	 {
		 tPlaceMoveButtonDesc.fX = 675.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 7;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go news
	 {
		 tPlaceMoveButtonDesc.fX = 740.f;
		 tPlaceMoveButtonDesc.fY = 100.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 14;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	// Go export
	 {
		 tPlaceMoveButtonDesc.fX = 740.f;
		 tPlaceMoveButtonDesc.fY = 370.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::DEVELOPER);

		 tPlaceMoveButtonDesc.iTextureNum = 12;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }


	 // Go Fountain
	 {
		 tPlaceMoveButtonDesc.fX = 310.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 19;
		 tPlaceMoveButtonDesc.iOutlineTextureNum = 1;
		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // Go Sea
	 {
		 tPlaceMoveButtonDesc.fX = 400.f;
		 tPlaceMoveButtonDesc.fY = 373.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 17;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // Go jeonMangDae
	 {
		 tPlaceMoveButtonDesc.fX = 565.f;
		 tPlaceMoveButtonDesc.fY = 60.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 22;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // Go park
	 {
		 tPlaceMoveButtonDesc.fX = 660.f;
		 tPlaceMoveButtonDesc.fY = 60.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 21;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // Go play
	 {
		 tPlaceMoveButtonDesc.fX = 760.f;
		 tPlaceMoveButtonDesc.fY = 175.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 20;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // Go cafe
	 {
		 tPlaceMoveButtonDesc.fX = 760.f;
		 tPlaceMoveButtonDesc.fY = 260.f;
		 tPlaceMoveButtonDesc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::APART);

		 tPlaceMoveButtonDesc.iTextureNum = 18;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACEID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }

		 if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			 static_cast<uint32_t>(PLACEID::ISLAND), TEXT("LayerTag_UI"),
			 static_cast<CButton*>(tmp))))
		 {
			 return E_FAIL;
		 }
	 }

	 // ProtoTag_UI_BackGround
	 {
		 CBackGround::BACKGROUND_DESC tBackGroundDesc = {};
		 tBackGroundDesc.fX = 480.f;
		 tBackGroundDesc.fY = 240.f;
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
	 
	 //// ProtoTag_UI_BackGround
	 //{
		// CLoadingBG::LOADINGBG_DESC tBackGroundDesc = {};
		// tBackGroundDesc.fX = 480.f;
		// tBackGroundDesc.fY = 240.f;
		// tBackGroundDesc.fCX = 960.f;
		// tBackGroundDesc.fCY = 480.f;
		// tBackGroundDesc.fSpeedPerSecond = 0.f;
		// tBackGroundDesc.fRotationPerSecond = XMConvertToRadians(0.f);
		// tBackGroundDesc.eRenderGroup = RENDER_GROUP::TOP_UI_BLEND;

		// tmp = m_pGameInstance->Proto_Get_Object_Clone(
		//	 static_cast<uint32_t>(PLACEID::STATIC),
		//	 TEXT("ProtoTag_UI_LoadingBG"),
		//	 &tBackGroundDesc);

		// if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		//	 static_cast<uint32_t>(PLACEID::ISLAND),
		//	 TEXT("LayerTag_UI"),
		//	 tmp)))
		// {
		//	 Safe_Release(tmp);
		//	 return E_FAIL;
		// }
	 //}
	 //// ProtoTag_UI_BackGround
	 //{
		// CLoadingBG::LOADINGBG_DESC tBackGroundDesc = {};
		// tBackGroundDesc.fX = 480.f;
		// tBackGroundDesc.fY = 240.f;
		// tBackGroundDesc.fCX = 960.f;
		// tBackGroundDesc.fCY = 480.f;
		// tBackGroundDesc.fSpeedPerSecond = 0.f;
		// tBackGroundDesc.fRotationPerSecond = XMConvertToRadians(0.f);
		// tBackGroundDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI_BLEND;

		// tmp = m_pGameInstance->Proto_Get_Object_Clone(
		//	 static_cast<uint32_t>(PLACEID::STATIC),
		//	 TEXT("ProtoTag_UI_LoadingBG"),
		//	 &tBackGroundDesc);

		// if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		//	 static_cast<uint32_t>(PLACEID::ISLAND),
		//	 TEXT("LayerTag_UI"),
		//	 tmp)))
		// {
		//	 Safe_Release(tmp);
		//	 return E_FAIL;
		// }
	 //}
	
	return S_OK;
}

HRESULT CPlace_Island::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Island::Ready_Camera_Layer()
{




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
