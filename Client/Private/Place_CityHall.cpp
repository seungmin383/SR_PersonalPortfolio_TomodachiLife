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
#include "MiiManager.h"
#include "Transmit_Strategy.h"
#include "PageButton.h"
#include "IconTexture.h"

#define UI1_BASE_POS_X 320.f
#define UI1_BASE_POS_Y 100.f
#define UI1BTN_GAP_X  160.f
#define UI1BTN_GAP_Y  160.f

#define HEAD_BASE_POS_X 300.f
#define HEAD_BASE_POS_Y 120.f

#define HAIR_BASE_POS_X 351.f
#define HAIR_BASE_POS_Y 120.f

#define BTNGAPX 98.f
#define BTNGAPY 85.f

#define PAGE_BASE_POS_X 72.f
#define PAGE_BASE_POS_Y 35.f
#define PAGEBTN_GAP_Y 50.f

#define PAGE_BASE_POS_X2 300.f
#define PAGE_BASE_POS_Y2 455.f
#define PAGEBTN_GAP_X2 180.f



/*
    TEXT("LayerTag_Actor"),
    TEXT("LayerTag_UIBG"),
    TEXT("LayerTag_UI1"),
    TEXT("LayerTag_FaceShape"),
    TEXT("LayerTag_FaceSkin"),
    TEXT("LayerTag_Hair"),
    TEXT("LayerTag_Eyebrow"),
    TEXT("LayerTag_Eyebrow1"),
    TEXT("LayerTag_Eyes"),
    TEXT("LayerTag_Eyes1"),
    TEXT("LayerTag_Eyes2"),
    TEXT("LayerTag_Eyes3"),
    TEXT("LayerTag_Nose"),
    TEXT("LayerTag_Mouth"),
    TEXT("LayerTag_Mouth1"),
    TEXT("LayerTag_Page"),
*/



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

		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;
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

			uint32_t iSlotNum = {};

			for (uint32_t i = 0; i < 6; ++i)
			{
				uint32_t row = iSlotNum % 3; // 0 1 2 0
				uint32_t col = iSlotNum / 3; // 0 0 1 1

				CCommonButton::COMMONBUTTON_DESC Desc = {};
				Desc.fX = UI1_BASE_POS_X + UI1BTN_GAP_X * row;
				Desc.fY = UI1_BASE_POS_Y + UI1BTN_GAP_Y * col;
				Desc.fCX = 150.f;
				Desc.fCY = 150.f;
				Desc.fSpeedPerSecond = 15.f;

				vector<_wstring> tmpEnable;
				tmpEnable.push_back(TEXT("LayerTag_FaceShape"));
				tmpEnable.push_back(TEXT("LayerTag_Page"));
				tmpEnable.push_back(TEXT("LayerTag_UIBG"));

				vector<_wstring> tmpDisable;
				tmpDisable.push_back(TEXT("LayerTag_UI1"));

				Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, PLACEID::CITYHALL, tmpDisable, tmpEnable);
				Desc.eTexturePlaceID = PLACEID::CITYHALL;
				Desc.eIconTexturePlaceID = PLACEID::CITYHALL;
				Desc.strTextureTag = TEXT("ProtoTag_Component_CMButtonTexture");
				Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
				Desc.iTextureIndex = 0.f;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
					TEXT("ProtoTag_UI_CMButton"), &Desc);

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
					TEXT("LayerTag_UI1"), tmp)))
				{
					Safe_Release(tmp);
					return E_FAIL;
				}

				++iSlotNum;


				CIconTexture::ICONTEXTURE_DESC IconDesc = {};
				IconDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
				IconDesc.fX = Desc.fX;
				IconDesc.fY = Desc.fY - 40.f;
				IconDesc.fZ = 0.4f;

				IconDesc.fCX = 120.f;
				IconDesc.fCY = 53.f;
				IconDesc.iTextureIndex = i;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
					TEXT("ProtoTag_UI_Icon"), &IconDesc);

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
					TEXT("LayerTag_UI1"), tmp)))
				{
					Safe_Release(tmp);
					return E_FAIL;
				}
			}

		}

		/* 저장 버튼 */
		{
			CCommonButton::COMMONBUTTON_DESC Desc = {};
				Desc.fX = 850.f;
				Desc.fY = 455.f;

				Desc.fCX = 141.f;
				Desc.fCY = 45.f;
				Desc.fSpeedPerSecond = 15.f;

				vector<_wstring> tmpEnable;
				tmpEnable.push_back(TEXT("LayerTag_FaceShape"));
				tmpEnable.push_back(TEXT("LayerTag_Page"));
				tmpEnable.push_back(TEXT("LayerTag_UIBG"));

				vector<_wstring> tmpDisable;
				tmpDisable.push_back(TEXT("LayerTag_UI1"));

				Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, PLACEID::CITYHALL, tmpDisable, tmpEnable);
				Desc.eTexturePlaceID = PLACEID::CITYHALL;
				Desc.eIconTexturePlaceID = PLACEID::CITYHALL;
				Desc.strTextureTag = TEXT("ProtoTag_Component_CMButtonTexture");
				Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
				Desc.iTextureIndex = 4;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
					TEXT("ProtoTag_UI_CMButton"), &Desc);

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
					TEXT("LayerTag_Page"), tmp)))
				{
					Safe_Release(tmp);
					return E_FAIL;
				}
		}
	}
#pragma region FACEEDITBUTTONS


	// LayerTag_FaceShape
	{
		// 얼굴형 변경 버튼
		{
			CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
			Desc.fX = HEAD_BASE_POS_X;
			Desc.fY = HEAD_BASE_POS_Y;
			Desc.fCX = 84.f;
			Desc.fCY = 72.f;
			Desc.fSpeedPerSecond = 30.f;
			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			Desc.iTextureIndex = 0;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 0);
			Desc.strLayerTag = L"LayerTag_FaceShape";

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y;
			Desc.iTextureIndex = 2;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 7);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}


			Desc.fX = HEAD_BASE_POS_X;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY;
			Desc.iTextureIndex = 4;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 6);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY;
			Desc.iTextureIndex = 6;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 5);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
				{
					Safe_Release(tmp);
					return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY * 2.f;
			Desc.iTextureIndex = 8;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 4);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY * 2.f;
			Desc.iTextureIndex = 10;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 3);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY * 3.f;
			Desc.iTextureIndex = 12;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 2);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			Desc.fX = HEAD_BASE_POS_X + BTNGAPX;
			Desc.fY = HEAD_BASE_POS_Y + BTNGAPY * 3.f;
			Desc.iTextureIndex = 14;
			Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HEAD, 1);
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("LayerTag_FaceShape"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_FaceShape"),
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}
		}
	}

	// LayerTag_FaceSkin
	{

	}

	/* 40 ~ 59 */
	// LayerTag_Hair
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 40;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 1);
		Desc.strLayerTag = L"LayerTag_Hair";

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X + BTNGAPX;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.iTextureIndex = 42;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 3);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X + BTNGAPX * 2;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.iTextureIndex = 44;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 5);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY;
		Desc.iTextureIndex = 46;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 7);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X + BTNGAPX;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY;
		Desc.iTextureIndex = 48;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 9);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X + BTNGAPX * 2;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY;
		Desc.iTextureIndex = 50;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 0);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * 2;
		Desc.iTextureIndex = 52;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 2);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X + BTNGAPX;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * 2;
		Desc.iTextureIndex = 54;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 6);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X + BTNGAPX * 2;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * 2;
		Desc.iTextureIndex = 56;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 8);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * 3;
		Desc.iTextureIndex = 58;
		Desc.pStrategy = CTransmit_Strategy::Create(m_pDevice, m_pContext, CTransmit_Strategy::PARTS::HAIR, 4);
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Hair"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Hair"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}
	}

	/* 60 ~ 107 눈썹 버튼 텍스쳐 */
	/* 78 ~ 100 눈썹 텍스쳐*/
	// LayerTag_Eyebrow
	// LayerTag_Eyebrow1
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 60;
		Desc.pStrategy = CTransmit_Strategy::Create(
			m_pDevice, m_pContext,
			CTransmit_Strategy::PARTS::EYEBROW, 78);   // 그리드상 1번(슬롯 0)
		Desc.strLayerTag = L"LayerTag_Eyebrow";

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Eyebrow"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Eyebrow"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		// =============================
		// 여기부터 2번, 3번, 4번 ... 채우는 부분
		// =============================

		uint32_t slotIndex = 1;  // 그리드 상에서 2번 버튼 자리부터 시작 (0 = 첫 버튼)
		uint32_t eyebrowIndex = 0;  // 79 + eyebrowIndex → 79, 80, 81, ...

		for (uint32_t i = 62; i < 107; i += 2)
		{
			// 그리드 상 위치 (3열 기준)
			uint32_t row = slotIndex / 3;   // 0,0,0,1,1,1,2,2,2,...
			uint32_t col = slotIndex % 3;   // 1,2,0,1,2,0,...

			Desc.fX = HAIR_BASE_POS_X + BTNGAPX * static_cast<_float>(col);
			Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * static_cast<_float>(row);

			Desc.iTextureIndex = i;
			Desc.pStrategy = CTransmit_Strategy::Create(
				m_pDevice, m_pContext,
				CTransmit_Strategy::PARTS::EYEBROW,
				79 + eyebrowIndex); // 79, 80, 81, ...

			_wstring LayerTag = L"LayerTag_Eyebrow";
			if (slotIndex > 11)
				LayerTag += to_wstring(1);

			// 레이어는 동일
			Desc.strLayerTag = LayerTag;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"),
				&Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				LayerTag, tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				LayerTag,
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			++slotIndex;     // 그리드 슬롯(2번→3번→4번…)
			++eyebrowIndex;  // 파츠 인덱스(79→80→81…)
		}
}

	// LayerTag_Eyes
	// LayerTag_Eyes1
	// LayerTag_Eyes2
	// LayerTag_Eyes3
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 108;
		Desc.pStrategy = CTransmit_Strategy::Create(
			m_pDevice, m_pContext,
			CTransmit_Strategy::PARTS::EYES, 0);   // 그리드상 1번(슬롯 0)
		Desc.strLayerTag = L"LayerTag_Eyes";

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Eyes"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Eyes"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		uint32_t slotIndex = 1;  
		uint32_t eyesIndex = 1;  
		uint32_t iNumLayer = 1;

		for (uint32_t i = 110; i < 203; i+=2)
		{
			uint32_t row = slotIndex / 3;
			uint32_t col = slotIndex % 3;

			Desc.fX = HAIR_BASE_POS_X + BTNGAPX * static_cast<_float>(col);
			Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * static_cast<_float>(row);

			Desc.iTextureIndex = i;
			Desc.pStrategy = CTransmit_Strategy::Create(
				m_pDevice, m_pContext,
				CTransmit_Strategy::PARTS::EYES,
				eyesIndex);

			uint32_t groupIdx = slotIndex / 12;    // 0,0,...,0,1,1,...,1,2,...

			_wstring LayerTag = L"LayerTag_Eyes";
			if (groupIdx > 0)
				LayerTag += to_wstring(groupIdx);  // 0→Eyes, 1→Eyes1, 2→Eyes2, 3→Eyes3
			Desc.strLayerTag = LayerTag;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"),
				&Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				LayerTag, tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				LayerTag,
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			++slotIndex;
			++eyesIndex; 
		}
	}

	// LayerTag_Nose
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 204;
		Desc.pStrategy = CTransmit_Strategy::Create(
			m_pDevice, m_pContext,
			CTransmit_Strategy::PARTS::NOSE, 60);   // 그리드상 1번(슬롯 0)
		Desc.strLayerTag = L"LayerTag_Nose";

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Nose"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Nose"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		uint32_t slotIndex = 1;  // 그리드 상에서 2번 버튼 자리부터 시작 (0 = 첫 버튼)
		uint32_t NoseIndex = 1;  // 79 + eyebrowIndex → 79, 80, 81, ...
		
		for (uint32_t i = 206; i < 228; i += 2)
		{
			// 그리드 상 위치 (3열 기준)
			uint32_t row = slotIndex / 3;   // 0,0,0,1,1,1,2,2,2,...
			uint32_t col = slotIndex % 3;   // 1,2,0,1,2,0,...

			Desc.fX = HAIR_BASE_POS_X + BTNGAPX * static_cast<_float>(col);
			Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * static_cast<_float>(row);

			Desc.iTextureIndex = i;
			Desc.pStrategy = CTransmit_Strategy::Create(
				m_pDevice, m_pContext,
				CTransmit_Strategy::PARTS::NOSE,
				60 +NoseIndex); // 79, 80, 81, ...


			Desc.strLayerTag = L"LayerTag_Nose";

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"),
				&Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				L"LayerTag_Nose", tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				L"LayerTag_Nose",
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			++slotIndex;     // 그리드 슬롯(2번→3번→4번…)
			++NoseIndex;  // 파츠 인덱스(79→80→81…)
		}
	}

	
	// LayerTag_Mouth
	// LayerTag_Mouth1
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fX = HAIR_BASE_POS_X;
		Desc.fY = HAIR_BASE_POS_Y;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 228;
		Desc.pStrategy = CTransmit_Strategy::Create(
			m_pDevice, m_pContext,
			CTransmit_Strategy::PARTS::MOUTH, 101);   // 그리드상 1번(슬롯 0)
		Desc.strLayerTag = L"LayerTag_Mouth";

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_MiiEditButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Mouth"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Mouth"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		uint32_t slotIndex = 1;  // 그리드 상에서 2번 버튼 자리부터 시작 (0 = 첫 버튼)
		uint32_t MouthIndex = 1;  // 79 + eyebrowIndex → 79, 80, 81, ...

		for (uint32_t i = 230; i < 262; i += 2)
		{
			// 그리드 상 위치 (3열 기준)
			uint32_t row = slotIndex / 3;   // 0,0,0,1,1,1,2,2,2,...
			uint32_t col = slotIndex % 3;   // 1,2,0,1,2,0,...

			Desc.fX = HAIR_BASE_POS_X + BTNGAPX * static_cast<_float>(col);
			Desc.fY = HAIR_BASE_POS_Y + BTNGAPY * static_cast<_float>(row);

			Desc.iTextureIndex = i;
			Desc.pStrategy = CTransmit_Strategy::Create(
				m_pDevice, m_pContext,
				CTransmit_Strategy::PARTS::MOUTH,
				101 + MouthIndex); 
			uint32_t groupIdx = slotIndex / 12;

			_wstring LayerTag = L"LayerTag_Mouth";
			if (groupIdx > 0)
				LayerTag += to_wstring(groupIdx);
			Desc.strLayerTag = LayerTag;


			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				TEXT("ProtoTag_UI_MiiEditButton"),
				&Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				LayerTag, tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
				static_cast<uint32_t>(PLACEID::CITYHALL),
				LayerTag,
				static_cast<CButton*>(tmp))))
			{
				return E_FAIL;
			}

			++slotIndex;     // 그리드 슬롯(2번→3번→4번…)
			++MouthIndex;  // 파츠 인덱스(79→80→81…)
		}
	}

	// LayerTag_Page
	{
		/* 머리 버튼 */
		CPageButton::PAGEBUTTON_DESC Desc = {};
		Desc.fX = PAGE_BASE_POS_X;
		Desc.fY = PAGE_BASE_POS_Y;
		Desc.fCX = 100.f;
		Desc.fCY = 60.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 0;
		vector<_wstring> DisableLayers;
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		vector<_wstring> EnableLayers;
		EnableLayers.push_back(TEXT("LayerTag_FaceShape"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext,(PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 머리카락 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y;
		Desc.iTextureIndex = 2;

		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		EnableLayers.clear();
		EnableLayers.push_back(TEXT("LayerTag_Hair"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);
		
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 눈썹 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y * 2;
		Desc.iTextureIndex = 4;

		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
	//	DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		EnableLayers.clear();
		EnableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		
		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);
		
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 눈 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y * 3;
		Desc.iTextureIndex = 6;
		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
		
		EnableLayers.clear();
		EnableLayers.push_back(TEXT("LayerTag_Eyes"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 코 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y * 4;
		Desc.iTextureIndex = 8;
		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		EnableLayers.clear(); 
		EnableLayers.push_back(TEXT("LayerTag_Nose"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 입 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y * 5;
		Desc.iTextureIndex = 10;
		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		EnableLayers.clear(); 
		EnableLayers.push_back(TEXT("LayerTag_Mouth"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 안경 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y * 6;
		Desc.iTextureIndex = 12;
		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		EnableLayers.clear();
		EnableLayers.push_back(TEXT("LayerTag_Hair"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 키 버튼 */
		Desc.fY = PAGE_BASE_POS_Y + PAGEBTN_GAP_Y * 7;
		Desc.iTextureIndex = 14;
		DisableLayers.clear();
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		EnableLayers.clear();
		EnableLayers.push_back(TEXT("LayerTag_Hair"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}
	}





	/* 아래 선택창 */
	{
		/* 얼굴 편집 버튼 */
		CPageButton::PAGEBUTTON_DESC Desc = {};
		Desc.fX = 135.f;
		Desc.fY = PAGE_BASE_POS_Y2;
		Desc.fCX = 93.f;
		Desc.fCY = 45.f;
		Desc.fSpeedPerSecond = 30.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 24;
		vector<_wstring> DisableLayers;
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));

		vector<_wstring> EnableLayers;
		EnableLayers.push_back(TEXT("LayerTag_FaceShape"));

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 프로필 편집 버튼 */
		Desc.fX = PAGE_BASE_POS_X2;
		Desc.fY = PAGE_BASE_POS_Y2;
		Desc.iTextureIndex = 26;
		DisableLayers.clear();
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));

		EnableLayers.clear();

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 목소리 편집 버튼 */
		Desc.fX = PAGE_BASE_POS_X2 + PAGEBTN_GAP_X2 ;
		Desc.fY = PAGE_BASE_POS_Y2;
		Desc.iTextureIndex = 28;
		DisableLayers.clear();
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));

		EnableLayers.clear();

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

		/* 성격 편집 버튼 */
		Desc.fX = PAGE_BASE_POS_X2 + PAGEBTN_GAP_X2 * 2;
		Desc.fY = PAGE_BASE_POS_Y2;
		Desc.iTextureIndex = 30;
		DisableLayers.clear();
		//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
		DisableLayers.push_back(TEXT("LayerTag_Hair"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
		DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
		DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
		DisableLayers.push_back(TEXT("LayerTag_Nose"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth"));
		DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
		DisableLayers.push_back(TEXT("LayerTag_FaceShape"));

		EnableLayers.clear();

		Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("ProtoTag_UI_PageButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
			TEXT("LayerTag_Page"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Page"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}
	}

#pragma endregion


	//
	const uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::CITYHALL);
	const _tchar* szDisableLayers[] =
	{
		// TEXT("LayerTag_FaceSkin"),
		TEXT("LayerTag_UIBG"),
		TEXT("LayerTag_Page"),
		TEXT("LayerTag_FaceShape"),
		TEXT("LayerTag_Hair"),
		TEXT("LayerTag_Eyebrow"),
		TEXT("LayerTag_Eyebrow1"),
		TEXT("LayerTag_Eyes"),
		TEXT("LayerTag_Eyes1"),
		TEXT("LayerTag_Eyes2"),
		TEXT("LayerTag_Eyes3"),
		TEXT("LayerTag_Nose"),
		TEXT("LayerTag_Mouth"),
		TEXT("LayerTag_Mouth1"),
		// 필요하면 여기다 다른 레이어도 추가
	};

	for (const _tchar* szTag : szDisableLayers)
		m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, szTag); return S_OK;
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
