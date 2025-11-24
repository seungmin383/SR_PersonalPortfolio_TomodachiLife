#include "Place_CityHall.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "CommonButton.h"
#include "ChangeUI_Strategy.h"
#include "MiiEditorBG.h"
#include "Mii.h"
#include "BackGround.h"
#include "MiiEditButton.h"
#include "ChangePlace_Strategy.h"

#define HEAD_BASE_POS_X 300.f
#define HEAD_BASE_POS_Y 120.f
#define BTNGAPX 98.f
#define BTNGAPY 85.f

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
	return 0;
}

HRESULT CPlace_CityHall::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("주민센터다."));
	return S_OK;
}

HRESULT CPlace_CityHall::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("주민센터다."));
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	// ProtoTag_Actor_EditorBG
	{
		CMiiEditorBG::MIIEDITORBG_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 5.f, 0.f };
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		Desc.fSpeedPerSecond = 10.f;
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_Actor_EditorBG"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;
	}

	{
		// ProtoTag_Actor_Mii
		{
			CMii::MII_DESC MiiDesc = {};
			MiiDesc.fX = 0.f;
			MiiDesc.fY = 0.f;
			MiiDesc.fZ = 0.f;
			MiiDesc.fCX = 1.f;
			MiiDesc.fCY = 1.f;
			MiiDesc.fCZ = 1.f;
			MiiDesc.fSpeedPerSecond =0.f;
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
	}

	return S_OK;
}

HRESULT CPlace_CityHall::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };

	// LayerTag_UIBG
	{
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

			tBackGroundDesc.eTexturePlaceID = PLACEID::CITYHALL;
			tBackGroundDesc.strTextureTag = TEXT("ProtoTag_Component_UIBGTexture");

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_BackGround"),
				&tBackGroundDesc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UIBG"),
				tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
		}
	}

	// LayerTag_UI1
	{
		/* mii 추가하는 버튼 */
		{
			CCommonButton::COMMONBUTTON_DESC Desc = {};
			Desc.fX = 210.f;
			Desc.fY = 340.f;
			Desc.fCX = 95.f;
			Desc.fCY = 81.4f;
			Desc.fSpeedPerSecond = 15.f;

			vector<_wstring> tmpEnable;
			tmpEnable.push_back(TEXT("LayerTag_UI2"));

			vector<_wstring> tmpDisable;
			tmpDisable.push_back(TEXT("LayerTag_UI1"));

			Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, PLACEID::CITYHALL, tmpDisable, tmpEnable);
			Desc.eTexturePlaceID = PLACEID::CITYHALL;
			Desc.eIconTexturePlaceID = PLACEID::CITYHALL;
			Desc.strTextureTag = TEXT("ProtoTag_Component_CMButtonTexture");
			Desc.strIconTextureTag = TEXT("ProtoTag_Component_IconTexture");
			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			Desc.iIconTextureIndex = 0;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_CMButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI1"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
		}
	}

	// LayerTag_UI2
	{
		// Head0
		{
			CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
			Desc.fX = HEAD_BASE_POS_X;
			Desc.fY = HEAD_BASE_POS_Y;
			Desc.fCX = 84.f;
			Desc.fCY = 72.f;
			Desc.fSpeedPerSecond = 30.f;
			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			Desc.iTextureIndex = 0;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 0;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y;
			Desc.iTextureIndex = 2;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 1;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}


			Desc.fX = HEAD_BASE_POS_X;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY;
			Desc.iTextureIndex = 4;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 2;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY;
			Desc.iTextureIndex = 6;
			Desc.iModelIndex = 3;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X ;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY*2.f;
			Desc.iTextureIndex = 8;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 4;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y + +BTNGAPY*2.f;
			Desc.iTextureIndex = 10;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 5;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X ;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY * 3.f;
			Desc.iTextureIndex = 12;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 6;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY * 3.f;
			Desc.iTextureIndex = 14;
			Desc.pStrategy = CChangePlace_Strategy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
			Desc.iModelIndex = 7;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_UI2"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}
		}
	}
	m_pGameInstance->ObjectManager_Disable_Layer(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_UI2"));
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 0.f, 4.f, -10.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 0.f, 4.f, 0.f, 1.f }, XMConvertToRadians(20.f),1.f );
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
