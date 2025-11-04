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

	return S_OK;
}

int32_t CPlace_Island::Update(_float fDT)
{

	return 0;
}

HRESULT CPlace_Island::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("¼¶ÀÌ´Ù."));
	return S_OK;
}

HRESULT CPlace_Island::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("¼¶ÀÌ´Ù."));
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
			static_cast<uint32_t>(PLACE_ID::ISLAND),
			TEXT("ProtoTag_Actor_Island"),
			&tIslandDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACE_ID::ISLAND),
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
			static_cast<uint32_t>(PLACE_ID::ISLAND),
			TEXT("ProtoTag_Actor_SkyAndSea"),
			&tSkyAndSeaDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACE_ID::ISLAND),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_Actor_Apart
	{
		CApart::APART_DESC tApartDesc = {};
		tApartDesc.fX = 0.f;
		tApartDesc.fY = 0.f;
		tApartDesc.fZ = 0.f;
		tApartDesc.fCX = 1.f;
		tApartDesc.fCY = 1.f;
		tApartDesc.fCZ = 1.f;
		tApartDesc.fSpeedPerSecond = 0.f;
		tApartDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tApartDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACE_ID::ISLAND),
			TEXT("ProtoTag_Actor_Apart"),
			&tApartDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACE_ID::ISLAND),
			TEXT("LayerTag_Actor"),
			tmp)))
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
	 	tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		tPlaceMoveButtonDesc.iTextureNum = 45;

	 	tmp = m_pGameInstance->Proto_Get_Object_Clone(
	 		static_cast<uint32_t>(PLACE_ID::ISLAND),
	 		TEXT("ProtoTag_UI_PlaceMoveButton"),
	 		&tPlaceMoveButtonDesc);
	 
	 	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
	 		static_cast<uint32_t>(PLACE_ID::ISLAND),
	 		TEXT("LayerTag_UI"),
	 		tmp)))
	 	{
	 		Safe_Release(tmp);
	 		return E_FAIL;
	 	}
	 }

	 // Go Wear
	 {
		 tPlaceMoveButtonDesc.fX = 270.f;
		 tPlaceMoveButtonDesc.fY = 70.f;
		 tPlaceMoveButtonDesc.fCX = 115.f;
		 tPlaceMoveButtonDesc.fCY = 115.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 0;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	 // Go Antique
	 {
		 tPlaceMoveButtonDesc.fX = 220.f;
		 tPlaceMoveButtonDesc.fY = 150.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 2;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	 // Go Cap
	 {
		 tPlaceMoveButtonDesc.fX = 225.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 3;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Food
	 {
		 tPlaceMoveButtonDesc.fX = 210.f;
		 tPlaceMoveButtonDesc.fY = 340.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 8;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Fountain
	 {
		 tPlaceMoveButtonDesc.fX = 310.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 19;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Luxury
	 {
		 tPlaceMoveButtonDesc.fX = 300.f;
		 tPlaceMoveButtonDesc.fY = 340.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 9;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Music
	 {
		 tPlaceMoveButtonDesc.fX = 400.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 4;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Sea
	 {
		 tPlaceMoveButtonDesc.fX = 400.f;
		 tPlaceMoveButtonDesc.fY = 373.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 17;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go House
	 {
		 tPlaceMoveButtonDesc.fX = 490.f;
		 tPlaceMoveButtonDesc.fY = 60.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 1;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go CityHall
	 {
		 tPlaceMoveButtonDesc.fX = 500.f;
		 tPlaceMoveButtonDesc.fY = 150.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 16;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Rank
	 {
		 tPlaceMoveButtonDesc.fX = 475.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 5;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go Tent
	 {
		 tPlaceMoveButtonDesc.fX = 475.f;
		 tPlaceMoveButtonDesc.fY = 340.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 10;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go jeonMangDae
	 {
		 tPlaceMoveButtonDesc.fX = 565.f;
		 tPlaceMoveButtonDesc.fY = 60.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 22;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go wedding
	 {
		 tPlaceMoveButtonDesc.fX = 600.f;
		 tPlaceMoveButtonDesc.fY = 150.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 15;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go love
	 {
		 tPlaceMoveButtonDesc.fX = 570.f;
		 tPlaceMoveButtonDesc.fY = 260.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 6;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go port
	 {
		 tPlaceMoveButtonDesc.fX = 600.f;
		 tPlaceMoveButtonDesc.fY = 375.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 11;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go park
	 {
		 tPlaceMoveButtonDesc.fX = 660.f;
		 tPlaceMoveButtonDesc.fY = 60.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 21;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go photo
	 {
		 tPlaceMoveButtonDesc.fX = 675.f;
		 tPlaceMoveButtonDesc.fY = 250.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 7;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go news
	 {
		 tPlaceMoveButtonDesc.fX = 740.f;
		 tPlaceMoveButtonDesc.fY = 100.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 14;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go play
	 {
		 tPlaceMoveButtonDesc.fX = 760.f;
		 tPlaceMoveButtonDesc.fY = 175.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 20;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go cafe
	 {
		 tPlaceMoveButtonDesc.fX = 760.f;
		 tPlaceMoveButtonDesc.fY = 260.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 18;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
			 return E_FAIL;
		 }
	 }

	// Go export
	 {
		 tPlaceMoveButtonDesc.fX = 740.f;
		 tPlaceMoveButtonDesc.fY = 370.f;
		 tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::APART;
		 tPlaceMoveButtonDesc.iTextureNum = 12;

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_PlaceMoveButton"),
			 &tPlaceMoveButtonDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("LayerTag_UI"),
			 tmp)))
		 {
			 Safe_Release(tmp);
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

		 tmp = m_pGameInstance->Proto_Get_Object_Clone(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
			 TEXT("ProtoTag_UI_BackGround"),
			 &tBackGroundDesc);

		 if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			 static_cast<uint32_t>(PLACE_ID::ISLAND),
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
