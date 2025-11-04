#include "Place_Island.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "Island.h"
#include "SkyAndSea.h"
#include "PlaceMoveButton.h"
#include "BackGround.h"

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

	return S_OK;
}

HRESULT CPlace_Island::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };

	 // ProtoTag_UI_PlaceMoveButton
	 {
	 	CPlaceMoveButton::PLACEMOVEBUTTON_DESC tPlaceMoveButtonDesc = {};
	 	tPlaceMoveButtonDesc.fX = 480.f;
	 	tPlaceMoveButtonDesc.fY = 240.f;
	 	tPlaceMoveButtonDesc.fCX = 100.f;
	 	tPlaceMoveButtonDesc.fCY = 100.f;
	 	tPlaceMoveButtonDesc.fSpeedPerSecond = 0.f;
	 	tPlaceMoveButtonDesc.fRotationPerSecond = XMConvertToRadians(20.f);
	 	tPlaceMoveButtonDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
	 	tPlaceMoveButtonDesc.eNextPlace = PLACE_ID::ISLAND;
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
