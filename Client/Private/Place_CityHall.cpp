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
#include "TextButton.h"
#include "SaveChangePlace_Strategy.h"
#include "MiiEdit_Strgy.h"
#include "CityhallMenu_Button.h"
#include "ControlEnable_Strgy.h"


#define CITYHALLMENU_BASE_POS_X 320.f
#define CITYHALLMENU_BASE_POS_Y 100.f
#define CITYHALLMENU_GAPX  160.f
#define CITYHALLMENU_GAPY  160.f


#define PAGE_BASE_POS_X 74.5f
#define PAGE_BASE_POS_Y 35.f
#define PAGEBTN_GAP_Y 52.f

#define PAGE_BASE_POS_X2 300.f
#define PAGE_BASE_POS_Y2 455.f
#define PAGEBTN_GAP_X2 180.f

#define TEXT_BASE_POS_X 300.f
#define TEXT_BASE_POS_Y 100.f


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
	}

	return S_OK;
}

HRESULT CPlace_CityHall::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::CITYHALL);

	// LayerTag_UIBG
	{

	}

	// LayerTag_UI_CityhallMenu
	{
		CCityhallMenu_Button::CITYHALLMENUBUTTON_DESC Desc = {};
		Desc.fCX = 150.f;
		Desc.fCY = 150.f;
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		CIconTexture::ICONTEXTURE_DESC IconDesc = {};
		IconDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		IconDesc.fCY = 53.f;
		IconDesc.fCX = 120.f;
		IconDesc.fZ = 0.4f;

		vector<_wstring> EnableLayers{
			TEXT("LayerTag_UI_CityhallMenu2")
		};
		vector<_wstring> DisableLayers{
			TEXT("LayerTag_UI_CityhallMenu")
		};

		uint32_t iIndex = { 0 };
		for (uint32_t i = 0; i < 2; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				/* 마을회관 메뉴 버튼 */
				Desc.fX = CITYHALLMENU_BASE_POS_X + (CITYHALLMENU_GAPX * j);
				Desc.fY = CITYHALLMENU_BASE_POS_Y + (CITYHALLMENU_GAPY * i);

				CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
				strgyDesc.EnableLayers = EnableLayers;
				strgyDesc.DisableLayers = DisableLayers;
				strgyDesc.ePlaceID = PLACEID::CITYHALL;
				strgyDesc.strLayerTag = TEXT("LayerTag_UI_CityhallMenu");

				Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);

				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_CityhallMenu_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_CityhallMenu"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_CityhallMenu"), static_cast<CButton*>(tmp))))
					return E_FAIL;

				/* 아이콘 텍스쳐 */
				IconDesc.fX = Desc.fX;
				IconDesc.fY = Desc.fY - 40.f;

				IconDesc.iTextureIndex = iIndex;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Icon"), &IconDesc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_CityhallMenu"), tmp)))
					return E_FAIL;

				++iIndex;
			}
		}
	}

	// LayerTag_UI_CityhallMenu2
	{
		CCityhallMenu_Button::CITYHALLMENUBUTTON_DESC Desc = {};
		Desc.fCX = 200.f;
		Desc.fCY = 200.f;
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		CIconTexture::ICONTEXTURE_DESC IconDesc = {};
		IconDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		IconDesc.fCX = 124;
		IconDesc.fCY = 164;
		IconDesc.fZ = 0.4f;

		uint32_t iIndex = { 0 };

		for (uint32_t i = 0; i < 2; ++i)
		{
			/* 마을회관 메뉴 버튼 */
			Desc.fX = 320.f + (320.f                                                                                                                * i);
			Desc.fY = 240.f;
			
			Desc.pClicked_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::MII_EDITOR);
			Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_CityhallMenu_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_CityhallMenu2"), tmp)))
				return E_FAIL;

			/* 아이콘 텍스쳐 */
			IconDesc.fX = Desc.fX;
			IconDesc.fY = Desc.fY;

			IconDesc.iTextureIndex = 6 + iIndex;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Icon"), &IconDesc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_CityhallMenu2"), tmp)))
				return E_FAIL;

			++iIndex;
		}
	}
		///* 저장 버튼 */
		//{
		//	CCommonButton::COMMONBUTTON_DESC Desc = {};
		//	Desc.fX = 850.f;
		//	Desc.fY = 455.f;

		//	Desc.fCX = 141.f;
		//	Desc.fCY = 45.f;
		//	Desc.fSpeedPerSecond = 15.f;

		//	Desc.pStrategy = CSaveChangePlace_Strategy::Create(m_pDevice, m_pContext);

		//	Desc.eTexturePlaceID = PLACEID::CITYHALL;
		//	Desc.eIconTexturePlaceID = PLACEID::CITYHALL;
		//	Desc.strTextureTag = TEXT("ProtoTag_Component_CMButtonTexture");
		//	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		//	Desc.iTextureIndex = 4;

		//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
		//		TEXT("ProtoTag_UI_CMButton"), &Desc);

		//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
		//		TEXT("LayerTag_Category"), tmp)))
		//	{
		//		Safe_Release(tmp);
		//		return E_FAIL;
		//	}

		//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
		//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Category"),
		//		static_cast<CButton*>(tmp))))
		//	{
		//		return E_FAIL;
		//	}

		//}

	



	///* 아래 선택창 */
	//{
	//	/* 얼굴 편집 버튼 */
	//	CPageButton::PAGEBUTTON_DESC Desc = {};
	//	Desc.fX = 135.f;
	//	Desc.fY = PAGE_BASE_POS_Y2;
	//	Desc.fCX = 93.f;
	//	Desc.fCY = 45.f;
	//	Desc.fSpeedPerSecond = 30.f;
	//	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
	//	Desc.iTextureIndex = 24;
	//	Desc.strDeactivateLayerTag = TEXT("LayerTag_Category");

	//	vector<_wstring> DisableLayers;
	//	//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
	//	DisableLayers.push_back(TEXT("LayerTag_Hair"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
	//	DisableLayers.push_back(TEXT("LayerTag_Nose"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
	//	DisableLayers.push_back(TEXT("LayerTag_TextButton"));

	//	vector<_wstring> EnableLayers;
	//	EnableLayers.push_back(TEXT("LayerTag_FaceShape"));
	//	EnableLayers.push_back(TEXT("LayerTag_Page"));

	//	Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

	//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("ProtoTag_UI_PageButton"), &Desc);

	//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("LayerTag_Category"), tmp)))
	//	{
	//		Safe_Release(tmp);
	//		return E_FAIL;
	//	}

	//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
	//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Category"),
	//		static_cast<CButton*>(tmp))))
	//	{
	//		return E_FAIL;
	//	}

	//	/* 프로필 편집 버튼 */
	//	Desc.fX = PAGE_BASE_POS_X2;
	//	Desc.fY = PAGE_BASE_POS_Y2;
	//	Desc.iTextureIndex = 26;
	//	DisableLayers.clear();
	//	//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));

	//	DisableLayers.push_back(TEXT("LayerTag_Hair"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
	//	DisableLayers.push_back(TEXT("LayerTag_Nose"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
	//	DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
	//	DisableLayers.push_back(TEXT("LayerTag_Page"));

	//	EnableLayers.clear();
	//	EnableLayers.push_back(TEXT("LayerTag_TextButton"));

	//	Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

	//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("ProtoTag_UI_PageButton"), &Desc);

	//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("LayerTag_Category"), tmp)))
	//	{
	//		Safe_Release(tmp);
	//		return E_FAIL;
	//	}

	//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
	//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Category"),
	//		static_cast<CButton*>(tmp))))
	//	{
	//		return E_FAIL;
	//	}

	//	/* 목소리 편집 버튼 */
	//	Desc.fX = PAGE_BASE_POS_X2 + PAGEBTN_GAP_X2;
	//	Desc.fY = PAGE_BASE_POS_Y2;
	//	Desc.iTextureIndex = 28;
	//	DisableLayers.clear();
	//	//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
	//	DisableLayers.push_back(TEXT("LayerTag_Hair"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
	//	DisableLayers.push_back(TEXT("LayerTag_Nose"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
	//	DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
	//	DisableLayers.push_back(TEXT("LayerTag_TextButton"));

	//	EnableLayers.clear();

	//	Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

	//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("ProtoTag_UI_PageButton"), &Desc);

	//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("LayerTag_Category"), tmp)))
	//	{
	//		Safe_Release(tmp);
	//		return E_FAIL;
	//	}

	//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
	//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Category"),
	//		static_cast<CButton*>(tmp))))
	//	{
	//		return E_FAIL;
	//	}

	//	/* 성격 편집 버튼 */
	//	Desc.fX = PAGE_BASE_POS_X2 + PAGEBTN_GAP_X2 * 2;
	//	Desc.fY = PAGE_BASE_POS_Y2;
	//	Desc.iTextureIndex = 30;
	//	DisableLayers.clear();
	//	//DisableLayers.push_back(TEXT("LayerTag_FaceSkin"));
	//	DisableLayers.push_back(TEXT("LayerTag_Hair"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyebrow1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes1"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes2"));
	//	DisableLayers.push_back(TEXT("LayerTag_Eyes3"));
	//	DisableLayers.push_back(TEXT("LayerTag_Nose"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth"));
	//	DisableLayers.push_back(TEXT("LayerTag_Mouth1"));
	//	DisableLayers.push_back(TEXT("LayerTag_FaceShape"));
	//	DisableLayers.push_back(TEXT("LayerTag_TextButton"));

	//	EnableLayers.clear();

	//	Desc.pStrategy = CChangeUI_Strategy::Create(m_pDevice, m_pContext, (PLACEID::CITYHALL), DisableLayers, EnableLayers);

	//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("ProtoTag_UI_PageButton"), &Desc);

	//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
	//		TEXT("LayerTag_Category"), tmp)))
	//	{
	//		Safe_Release(tmp);
	//		return E_FAIL;
	//	}

	//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
	//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Category"),
	//		static_cast<CButton*>(tmp))))
	//	{
	//		return E_FAIL;
	//	}
	//}


///* 텍스트 입력 버튼 */
////LayerTag_TextButton
//{
//	/* 성 */
//	CTextButton::TEXTBUTTON_DESC Desc = {};
//	Desc.fX = TEXT_BASE_POS_X;
//	Desc.fY = TEXT_BASE_POS_Y;
//	Desc.fCX = 200.f;
//	Desc.fCY = 33.f;
//	Desc.fSpeedPerSecond = 30.f;
//	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
//	Desc.pStrategy = nullptr;
//	Desc.eTextType = CTextButton::TEXTTYPE::FIRSTNAME;

//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
//		TEXT("ProtoTag_UI_TextButton"), &Desc);

//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
//		TEXT("LayerTag_TextButton"), tmp)))
//	{
//		Safe_Release(tmp);
//		return E_FAIL;
//	}

//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_TextButton"),
//		static_cast<CButton*>(tmp))))
//	{
//		return E_FAIL;
//	}

//	/* 이름 */
//	Desc.fX = TEXT_BASE_POS_X + 300.f;
//	Desc.fY = TEXT_BASE_POS_Y;
//	Desc.eTextType = CTextButton::TEXTTYPE::LASTNAME;

//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
//		TEXT("ProtoTag_UI_TextButton"), &Desc);

//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
//		TEXT("LayerTag_TextButton"), tmp)))
//	{
//		Safe_Release(tmp);
//		return E_FAIL;
//	}


//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_TextButton"),
//		static_cast<CButton*>(tmp))))
//	{
//		return E_FAIL;
//	}

//	/* 닉네임 */
//	Desc.fX = TEXT_BASE_POS_X;
//	Desc.fY = TEXT_BASE_POS_Y + 100.f;
//	Desc.eTextType = CTextButton::TEXTTYPE::NICKNAME;

//	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL),
//		TEXT("ProtoTag_UI_TextButton"), &Desc);

//	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL),
//		TEXT("LayerTag_TextButton"), tmp)))
//	{
//		Safe_Release(tmp);
//		return E_FAIL;
//	}

//	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
//		static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_TextButton"),
//		static_cast<CButton*>(tmp))))
//	{
//		return E_FAIL;
//	}
//}

//const _tchar* szDisableLayers[] =
//{
//	// TEXT("LayerTag_FaceSkin"),
//	TEXT("LayerTag_UIBG"),
//	TEXT("LayerTag_Page"),
//	TEXT("LayerTag_FaceShape"),
//	TEXT("LayerTag_Hair"),
//	TEXT("LayerTag_Eyebrow"),
//	TEXT("LayerTag_Eyebrow1"),
//	TEXT("LayerTag_Eyes"),
//	TEXT("LayerTag_Eyes1"),
//	TEXT("LayerTag_Eyes2"),
//	TEXT("LayerTag_Eyes3"),
//	TEXT("LayerTag_Nose"),
//	TEXT("LayerTag_Mouth"),
//	TEXT("LayerTag_Mouth1"),
//	TEXT("LayerTag_TextButton"),
//	TEXT("LayerTag_Category")
//	// 필요하면 여기다 다른 레이어도 추가
//};

{
	vector<_wstring	> DisableLayer{
		TEXT("LayerTag_UI_CityhallMenu2")
	};

	for(auto& str : DisableLayer)
		m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);

}
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
