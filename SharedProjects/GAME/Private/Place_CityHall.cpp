#include "Place_CityHall.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "TestMii.h"
#include "Terrain.h"
#include "Player.h"


CPlace_CityHall::CPlace_CityHall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_CityHall::Initialize()
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

int32_t CPlace_CityHall::Update(_float fDT)
{
	int a = 0;
	return 0;
}

HRESULT CPlace_CityHall::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("ÁÖ¹Î¼¾ÅÍ´Ù."));
	return S_OK;
}

HRESULT CPlace_CityHall::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("ÁÖ¹Î¼¾ÅÍ´Ù."));
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	// Actor
	{
		// ProtoTag_Actor_Mii
		{
			CTestMii::TEST_MII_DESC tMiiDesc = {};
			tMiiDesc.fX = 0.f;
			tMiiDesc.fY = 0.f;
			tMiiDesc.fZ = 0.f;
			tMiiDesc.fCX = 1.f;
			tMiiDesc.fCY = 1.f;
			tMiiDesc.fCZ = 1.f;
			tMiiDesc.fSpeedPerSecond = 10.f;
			tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);
			tMiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACE_ID::CITYHALL),
				TEXT("ProtoTag_Actor_Fiona"),
				&tMiiDesc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACE_ID::CITYHALL),
				TEXT("LayerTag_Actor"),
				tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
		}
		
		//ProtoTag_Actor_Terrain
		{
			CTerrain::ACTOR_DESC tTerrainDesc = {};
			tTerrainDesc.fX = 0.f;
			tTerrainDesc.fY = 0.f;
			tTerrainDesc.fZ = 0.f;
			tTerrainDesc.fCX = 1.f;
			tTerrainDesc.fCY = 1.f;
			tTerrainDesc.fCZ = 1.f;
			tTerrainDesc.fSpeedPerSecond = 10.f;
			tTerrainDesc.fRotationPerSecond = XMConvertToRadians(20.f);
			tTerrainDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACE_ID::CITYHALL),
				TEXT("ProtoTag_Actor_Terrain"),
				&tTerrainDesc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACE_ID::CITYHALL),
				TEXT("LayerTag_Actor"),
				tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
		}
	}

	// Component
	{

	}

	//UI
	{

	}
	
	

	
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };

	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Camera_Layer()
{
	return S_OK;
}


CPlace_CityHall* CPlace_CityHall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_CityHall* pPlaceMain = new CPlace_CityHall(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_CityHall::Free()
{
	CPlace::Free();
}
