#include "Place_Developer.h"

#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "TestMii.h"
#include "Terrain.h"
#include "Player.h"
#include "Mii.h"

#include "PlaceMoveButton.h"

CPlace_Developer::CPlace_Developer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Developer::Initialize()
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

int32_t CPlace_Developer::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_Developer::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("¡÷πŒºæ≈Õ¥Ÿ."));
	return S_OK;
}

HRESULT CPlace_Developer::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("¡÷πŒºæ≈Õ¥Ÿ."));
	return S_OK;
}

HRESULT CPlace_Developer::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	// ProtoTag_Actor_Mii
	{
		for (uint32_t i = 0; i < 20; ++i)
		{
			CTestMii::TEST_MII_DESC tMiiDesc = {};
			tMiiDesc.fX = m_pGameInstance->Random(0.f, 20.f);
			tMiiDesc.fY = 0.f;
			tMiiDesc.fZ = m_pGameInstance->Random(0.f, 20.f);
			tMiiDesc.fCX = 1.f;
			tMiiDesc.fCY = 1.f;
			tMiiDesc.fCZ = 1.f;
			tMiiDesc.fSpeedPerSecond = 10.f;
			tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);
			tMiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_Actor_Fiona"),
				&tMiiDesc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_Actor"),
				tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
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
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_Actor_Terrain"),
			&tTerrainDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_Actor_Mii
	{
		CMii::MII_DESC MiiDesc = {};
		MiiDesc.fX = 0.f;
		MiiDesc.fY = 0.f;
		MiiDesc.fZ = 0.f;
		MiiDesc.fCX = 1.f;
		MiiDesc.fCY = 1.f;
		MiiDesc.fCZ = 1.f;
		MiiDesc.fSpeedPerSecond = 15.f;
		MiiDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiDesc.fRotationPerSecond = XMConvertToRadians(90.f);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::STATIC),
			TEXT("ProtoTag_Actor_Mii"),
			&MiiDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPlace_Developer::Ready_UI_Layer()
{
	return S_OK;
}

HRESULT CPlace_Developer::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Developer::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	return S_OK;
}


CPlace_Developer* CPlace_Developer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Developer* pPlaceMain = new CPlace_Developer(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Developer::Free()
{
	CPlace::Free();
}
