#include "Place_Developer.h"

#include "GameInstance.h"
#include "Place_Loading.h"
#include "UICamera.h"
#include "WorldCamera.h"

#include "Explosion.h"
#include "Mii.h"
#include "Player.h"
#include "Snow.h"
#include "Terrain.h"
#include "TestMii.h"

#include "PlaceMoveButton.h"
#include "Back_Button.h"
#include "ChangePlace_Strategy.h"

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
	uint32_t iDevelop = static_cast<uint32_t>(PLACEID::DEVELOPER);

	// ProtoTag_Actor_Mii
	{
		for (uint32_t i = 0; i < 20; ++i)
		{
			CTestMii::TEST_MII_DESC tMiiDesc = {};
			tMiiDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
			tMiiDesc.fCX = 1.f;
			tMiiDesc.fCY = 1.f;
			tMiiDesc.fCZ = 1.f;
			tMiiDesc.fSpeedPerSecond = 10.f;
			tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);
			tMiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				iDevelop,
				TEXT("ProtoTag_Actor_Fiona"),
				&tMiiDesc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				iDevelop,
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
		tTerrainDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		tTerrainDesc.fCX = 1.f;
		tTerrainDesc.fCY = 1.f;
		tTerrainDesc.fCZ = 1.f;
		tTerrainDesc.fSpeedPerSecond = 10.f;
		tTerrainDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tTerrainDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			iDevelop,
			TEXT("ProtoTag_Actor_Terrain"),
			&tTerrainDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			iDevelop,
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
		MiiDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };

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
			iDevelop,
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	/* ¥´ */
	// ProtoTag_Actor_Snow
	{
		CSnow::SNOW_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		Desc.fCX = 1.f;
		Desc.fCY = 1.f;
		Desc.fCZ = 1.f;
		Desc.vPos = XMFLOAT4{ 10.f, 15.f, 10.f, 1.f };

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iDevelop,TEXT("ProtoTag_Actor_Snow"),&Desc);
		if (nullptr == tmp)
			return E_FAIL;	
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iDevelop,	TEXT("LayerTag_Actor"),	tmp)))
			return E_FAIL;
	}

	/* ∆¯πﬂ */
	// ProtoTag_Actor_Explosion
	{
		CExplosion::EXPLOSION_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		Desc.fCX = 1.f;
		Desc.fCY = 1.f;
		Desc.fCZ = 1.f;
		Desc.vPos = XMFLOAT4{ 0.f, 5.f, 0.f, 1.f };

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iDevelop,TEXT("ProtoTag_Actor_Explosion"),&Desc);
		if (nullptr == tmp)
			return E_FAIL;
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iDevelop, TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlace_Developer::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	/* µ⁄∑Œ∞°±‚ πˆ∆∞ */
	// LayerTag_UI_Back_Button
	{
		CBack_Button::BACKBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 100.f;
		Desc.fCY = 68.f;
		Desc.fX = 50.f;
		Desc.fY = 446.f;
		Desc.fZ = 0.5f;
		Desc.pClicked_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
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

	return S_OK;
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
