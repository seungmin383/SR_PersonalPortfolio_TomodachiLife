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
#include "PageButton.h"
#include "IconTexture.h"
#include "TextButton.h"
#include "MiiEdit_Strgy.h"
#include "CityhallMenu_Button.h"
#include "ControlEnable_Strgy.h"
#include "BeginMiiEdit.h"


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
	//uint32_t iPlaceID = 
	// ProtoTag_Actor_EditorBG
	{
		CMiiEditorBG::MIIEDITORBG_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 5.f, 0.f };
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		Desc.fSpeedPerSecond = 10.f;
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_EditorBG"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;

		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_EditorBG"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;
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
		_wstring strTextArr[] = {
			TEXT("신규 등록"),
			TEXT("주민 목록"),
			TEXT("아이 기록"),
			TEXT("앨범"),
			TEXT("보내기/받기"),
			TEXT("QR Code"),
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
				Desc.strText = strTextArr[iIndex];

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

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Icon"), &IconDesc);
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

		/* 남자인지 여자인지 */
		for (uint32_t i = 0; i < 2; ++i)
		{
			Desc.fX = 320.f + (320.f * i);
			Desc.fY = 240.f;

			if(i == 0)
				Desc.pClicked_Strgy = CBeginMiiEdit_Strgy::Create(m_pDevice, m_pContext, TRUE);
			else if(i == 1)
				Desc.pClicked_Strgy = CBeginMiiEdit_Strgy::Create(m_pDevice, m_pContext, FALSE);

			Desc.pSelected_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::MII_EDITOR);
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

	{
		vector<_wstring> EnableLayer{
			TEXT("LayerTag_UI_CityhallMenu")
		};

		vector<_wstring	> DisableLayer{
			TEXT("LayerTag_UI_CityhallMenu2")
		};

		for (auto& str : EnableLayer)
			m_pGameInstance->ObjectManager_Enable_Layer(iPlaceID, str);
		for (auto& str : DisableLayer)
			m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);
	
	}
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Environment_Layer()
{
	LIGHT_DESC Desc = {};
	Desc.eType = LIGHT_TYPE::DIRECTIONAL;
	Desc.vDir = _float4(-0.f, 1.f, 3.f, 0.f);
	Desc.vDiffuse = _float4(1.00f, 0.86f, 0.55f, 1.0f);
	Desc.vAmbient = _float4(0.20f, 0.17f, 0.10f, 1.0f);
	Desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

	Desc.eType = LIGHT_TYPE::POINT;
	Desc.vPos = _float4(4.f, 14.5f, -58.f, 1.f);
	Desc.fRange = 1.f;
	Desc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.0f);
	Desc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	Desc.vSpecular = Desc.vDiffuse;

	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;
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
