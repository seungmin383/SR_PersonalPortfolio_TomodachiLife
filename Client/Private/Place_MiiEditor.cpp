#include "Place_MiiEditor.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"
#include "MiiEditorBG.h"
#include "BackGround.h"
#include "MiiEditButton.h"

#include "Mii.h"
#include "MiiEdit_Strgy.h"
#include "PageButton.h"
#include "ControlEnable_Strgy.h"
#include "Option_Button.h"
#include "SaveMiiData_Strgy.h"
#include "ChangePlace_Strategy.h"
#include "TextButton.h"
#include "Deactive_Strgy.h"
#include "YellowBackGround.h"
#include "NoneBack_TextBox.h"
#include "EditOption_BackGround.h"
#include "IconTexture.h"
#include "CityhallMenu_Button.h"
#include "Category_Button.h"
#include "SetUIisEnable_Strgy.h"
#include "SetEditInfo_isMan_Strgy.h"
#include "Voice_Button.h"
#include "Personal_Base_BackGround.h"
#include "Personal_Button.h"
#include "Personal_Sub_BackGround.h"
#include "SetMiiPersonal_Strgy.h"

#define HEAD_BASE_POS_X 300.f
#define HEAD_BASE_POS_Y 120.f

#define HAIR_BASE_POS_X 351.f
#define HAIR_BASE_POS_Y 120.f

#define BTNGAPX 98.f
#define BTNGAPY 85.f

#define PAGE_BASE_POS_X 74.5f
#define PAGE_BASE_POS_Y 35.f
#define PAGEBTN_GAP_Y 52.f

#define CATEGORY_BASE_POS_X 300.f
#define CATEGORY_BASE_POS_Y 455.f
#define CATEGORYBTN_GAP_X 180.f

#define TEXT_BASE_POS_X 300.f
#define TEXT_BASE_POS_Y 100.f

CPlace_MiiEditor::CPlace_MiiEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_MiiEditor::Initialize()
{
	if (FAILED(Ready_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Camera_Layer()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->SoundManager_Begin_BGM(TEXT("BGM_MiiEditor"), 1.f)))
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_MiiEditor::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_MiiEditor::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("mii Editor"));
	return S_OK;
}

HRESULT CPlace_MiiEditor::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("mii Editor"));
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);;

	// ProtoTag_Actor_Mii
	{
		CMii::MII_DESC MiiDesc = {};
		MiiDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0, 1.f };
		MiiDesc.fCX = 1.f;
		MiiDesc.fCY = 1.f;
		MiiDesc.fCZ = 1.f;
		MiiDesc.fSpeedPerSecond = 0.f;
		MiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		MiiDesc.fRotationPerSecond = XMConvertToRadians(90.f);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC),
			TEXT("ProtoTag_Actor_Mii"),	&MiiDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,
			TEXT("LayerTag_Actor"),	tmp)))
			return E_FAIL;
	}


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
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;

	}
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_UI_Layer()
{
	uint32_t iPlaceID = static_cast<uint32_t >(PLACEID::MII_EDITOR);

	if (FAILED(Layer_UI_BackGround()))
		return E_FAIL;

	if (FAILED(Layer_UI_MiiEdit_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Page_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Category_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Save_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Text_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Voice_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Personal_Button()))
		return E_FAIL;


	vector<_wstring> EnableLayers{
		TEXT("LayerTag_UI_FaceShape"),
		TEXT("LayerTag_UI_Page_Button")
	};

	vector<_wstring> DisableLayers{
		TEXT("LayerTag_UI_Hair"),
		TEXT("LayerTag_UI_Eyebrow"),
		TEXT("LayerTag_UI_Eyes"),
		TEXT("LayerTag_UI_Nose"),
		TEXT("LayerTag_UI_Mouth"),
		TEXT("LayerTag_UI_Text_Button"),
		TEXT("LayerTag_UI_Voice_Button"),
		TEXT("LayerTag_UI_Personal_Action"),
		TEXT("LayerTag_UI_Personal_Accent"),
		TEXT("LayerTag_UI_Personal_Face"),
		TEXT("LayerTag_UI_Personal_Think"),
		TEXT("LayerTag_UI_Personal_Total"),
	};

	for (auto& str : EnableLayers)
		m_pGameInstance->ObjectManager_Enable_Layer(iPlaceID, str);
	for (auto& str : DisableLayers)
		m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);

	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_BackGround()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t> (PLACEID::MII_EDITOR);

	// ProtoTag_UI_BackGround
	{
		CBackGround::BACKGROUND_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 240.f;
		Desc.fZ = 1.f;
		Desc.fCX = 960.f;
		Desc.fCY = 480.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		Desc.eTexturePlaceID = PLACEID::MII_EDITOR;
		Desc.strTextureTag = TEXT("ProtoTag_Component_UI_BackGround_Texture");

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_BackGround"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,	TEXT("LayerTag_UI_BackGround"), tmp)))
			return E_FAIL;
	}



	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_MiiEdit_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t> (PLACEID::MII_EDITOR);

	// LayerTag_UI_FaceShape
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};

		Desc.fZ = 0.4f;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		uint32_t iIndexArr[] = {
			0,7,6,5,4,3,2,1
		};

		uint32_t iIndex = { 0 };

		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 2; ++j)
			{
				Desc.fX = HEAD_BASE_POS_X + (BTNGAPX * j);
				Desc.fY = HEAD_BASE_POS_Y + (BTNGAPY * i);
				Desc.pClicked_Strgy = CMiiEdit_Strgy::Create(m_pDevice, m_pContext, TEXT("LayerTag_UI_FaceShape"), CMiiEdit_Strgy::PARTS::HEAD, iIndexArr[iIndex]);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.iTextureIndex = 2 * iIndex;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), &Desc);
				if (tmp == nullptr)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_FaceShape"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_FaceShape"), static_cast<CButton*>(tmp))))
					return E_FAIL;
				++iIndex;
			}
		}
	}
	// LayerTag_UI_Hair
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fZ = 0.4f;

		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		uint32_t iIndexArr[] = {
			1,3,5,7,9,0,2,6,8,4
		};
		uint32_t iIndex = { 0 };

		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				if (iIndex > 9)
					continue;
				Desc.fX = HAIR_BASE_POS_X + (BTNGAPX * j);
				Desc.fY = HAIR_BASE_POS_Y + (BTNGAPY * i);
				Desc.pClicked_Strgy = CMiiEdit_Strgy::Create(m_pDevice, m_pContext, TEXT("LayerTag_UI_Hair"), CMiiEdit_Strgy::PARTS::HAIR, iIndexArr[iIndex]);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.iTextureIndex = 40 + (2 * iIndex);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), &Desc);
				if (tmp == nullptr)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Hair"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Hair"), static_cast<CButton*>(tmp))))
					return E_FAIL;

				++iIndex;
			}
		}
	}
	// LayerTag_UI_Eyebrow
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fZ = 0.4f;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		uint32_t iIndex = { 0 };
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				Desc.fX = HAIR_BASE_POS_X + (BTNGAPX * j);
				Desc.fY = HAIR_BASE_POS_Y + (BTNGAPY * i);
				Desc.pClicked_Strgy = CMiiEdit_Strgy::Create(m_pDevice, m_pContext, TEXT("LayerTag_UI_Eyebrow"), CMiiEdit_Strgy::PARTS::EYEBROW, 78 + iIndex);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.iTextureIndex = 60 + (2 * iIndex);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Eyebrow"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Eyebrow"), static_cast<CButton*>(tmp))))
					return E_FAIL;

				++iIndex;
			}
		}
	}
	// LayerTag_UI_Eyes
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fZ = 0.4f;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		uint32_t iIndex = { 0 };
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				Desc.fX = HAIR_BASE_POS_X + (BTNGAPX * j);
				Desc.fY = HAIR_BASE_POS_Y + (BTNGAPY * i);
				Desc.pClicked_Strgy = CMiiEdit_Strgy::Create(m_pDevice, m_pContext, TEXT("LayerTag_UI_Eyes"), CMiiEdit_Strgy::PARTS::EYES, iIndex);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.iTextureIndex = 108 + (2 * iIndex);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Eyes"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Eyes"), static_cast<CButton*>(tmp))))
					return E_FAIL;

				++iIndex;
			}
		}
	}
	// LayerTag_Nose
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fZ = 0.4f;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		uint32_t iIndex = { 0 };
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				Desc.fX = HAIR_BASE_POS_X + (BTNGAPX * j);
				Desc.fY = HAIR_BASE_POS_Y + (BTNGAPY * i);
				Desc.pClicked_Strgy = CMiiEdit_Strgy::Create(m_pDevice, m_pContext, TEXT("LayerTag_UI_Nose"), CMiiEdit_Strgy::PARTS::NOSE, 60 + iIndex);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.iTextureIndex = 204 + (2 * iIndex);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Nose"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Nose"), static_cast<CButton*>(tmp))))
					return E_FAIL;

				++iIndex;
			}
		}
	}
	// LayerTag_UI_Mouth
	{
		CMiiEditButton::MIIEDITBUTTON_DESC Desc = {};
		Desc.fZ = 0.4f;
		Desc.fCX = 84.f;
		Desc.fCY = 72.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		uint32_t iIndex = { 0 };
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				Desc.fX = HAIR_BASE_POS_X + (BTNGAPX * j);
				Desc.fY = HAIR_BASE_POS_Y + (BTNGAPY * i);
				Desc.pClicked_Strgy = CMiiEdit_Strgy::Create(m_pDevice, m_pContext, TEXT("LayerTag_UI_Mouth"), CMiiEdit_Strgy::PARTS::MOUTH, 101 + iIndex);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.iTextureIndex = 228 + (2 * iIndex);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_MiiEditButton"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Mouth"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Mouth"), static_cast<CButton*>(tmp))))
					return E_FAIL;

				++iIndex;
			}
		}
	}

	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_Page_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t> (PLACEID::MII_EDITOR);

	// LayerTag_UI_Page_Button
	/* 머리 버튼 */

	CPageButton::PAGEBUTTON_DESC Desc = {};
	Desc.fX = PAGE_BASE_POS_X;
	Desc.fZ = 0.5f;
	Desc.fCX = 95.f;
	Desc.fCY = 50.f;
	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

	vector<_wstring> LayerTagArr{
		TEXT("LayerTag_UI_FaceShape"),
		TEXT("LayerTag_UI_Hair"),
		TEXT("LayerTag_UI_Eyebrow"),
		TEXT("LayerTag_UI_Eyes"),
		TEXT("LayerTag_UI_Nose"),
		TEXT("LayerTag_UI_Mouth")
	};

	for (uint32_t i = 0; i < 8; ++i)
	{
		Desc.fY = PAGE_BASE_POS_Y + (PAGEBTN_GAP_Y * i);
		Desc.iTextureIndex = 2 * i;

		if (6 == i || 7 == i)
		{
			Desc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		}
		else
		{
			CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};

			strgyDesc.ePlaceID = PLACEID::MII_EDITOR;
			strgyDesc.EnableLayers = vector<_wstring>{ LayerTagArr[i] };
			strgyDesc.strLayerTag = TEXT("LayerTag_UI_Page_Button");
			vector<_wstring> DisableLayers;
			for (uint32_t j = 0; j < LayerTagArr.size(); ++j)
			{
				if(j != i)
					DisableLayers.push_back(LayerTagArr[j]);
			}
			strgyDesc.DisableLayers = DisableLayers;

			Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
		}

		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_PageButton"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Page_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Page_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_Category_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);;

	CPageButton::PAGEBUTTON_DESC Desc = {};
	Desc.fY = CATEGORY_BASE_POS_Y;
	Desc.fZ = 0.5f;
	Desc.fCX = 93.f;
	Desc.fCY = 45.f;
	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

	vector<_wstring> LayerTagArr{
		TEXT("LayerTag_UI_Page_Button"),	/* 0 */
		TEXT("LayerTag_UI_FaceShape"),
		TEXT("LayerTag_UI_Hair"),
		TEXT("LayerTag_UI_Eyebrow"),
		TEXT("LayerTag_UI_Eyes"),
		TEXT("LayerTag_UI_Nose"),			/* 5 */
		TEXT("LayerTag_UI_Mouth"),
		TEXT("LayerTag_UI_Text_Button"),
		TEXT("LayerTag_UI_Voice_Button"),

		TEXT("LayerTag_UI_Personal_Action"),
		TEXT("LayerTag_UI_Personal_Accent"),	/* 10 */
		TEXT("LayerTag_UI_Personal_Face"),
		TEXT("LayerTag_UI_Personal_Think"),
		TEXT("LayerTag_UI_Personal_Total"),

	};

	vector<uint32_t> EnableLayerIndex[] = {
		vector<uint32_t>{0, 1},
		vector<uint32_t>{7, },
		vector<uint32_t>{8},
		vector<uint32_t>{9,10,11,12,13}
	};

	vector<uint32_t> DisableLayerIndex[] = {
			vector<uint32_t>{2,3,4,5,6,7, 8, 9,10,11,12,13},
			vector<uint32_t>{0,1,2,3,4,5,6,8,9,10,11,12,13},
			vector<uint32_t>{0,1,2,3,4,5,6,7, 9,10,11,12,13},
			vector<uint32_t>{0,1,2,3,4,5,6,7,8}
	};


	for (uint32_t i = 0; i < 4; ++i)
	{
		if (i == 0)
			Desc.fX = 135.f;
		else
			Desc.fX = CATEGORY_BASE_POS_X + (CATEGORYBTN_GAP_X * (i - 1));

		Desc.iTextureIndex = 24 + 2 * i;

		CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
		strgyDesc.ePlaceID = PLACEID::MII_EDITOR;

		vector<_wstring> EnableLayers;
		for (uint32_t Idx : EnableLayerIndex[i])
			EnableLayers.push_back(LayerTagArr[Idx]);

		vector<_wstring> DisableLayers;
		for (uint32_t Idx : DisableLayerIndex[i])
			DisableLayers.push_back(LayerTagArr[Idx]);

		strgyDesc.EnableLayers = EnableLayers;
		strgyDesc.DisableLayers = DisableLayers;
		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Category_Button");
		Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);

		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_PageButton"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Category_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Category_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_Save_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);;

	//LayerTag_UI_Save_Button
	{
		COption_Button::OPTIONBUTTON_DESC Desc = {};
		Desc.fX = 875.f;
		Desc.fY = 455.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 170.f;
		Desc.fCY = 48.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.strText = TEXT("등록");
		Desc.strSoundTag = TEXT("SFX_Button_Click_Enter");

		//Desc.pClicked_Strgy = CSaveMiiData_Strgy::Create(m_pDevice, m_pContext);
		Desc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelected_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::SHOW_MII);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Option_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Save_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Save_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_Text_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

	XMFLOAT4 PosSizeArr[] = {
		XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y,			300.f,	50.f},	/* 성 */
		XMFLOAT4{TEXT_BASE_POS_X + 350.f,	TEXT_BASE_POS_Y,			300.f,	50.f},	/* 이름 */
		XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y + 100.f,	300.f,	50.f},	/* 닉네임 */

		XMFLOAT4{TEXT_BASE_POS_X + 275.f,	TEXT_BASE_POS_Y + 100.f,	150.f,	50.f},	/* 성별 */
		XMFLOAT4{TEXT_BASE_POS_X + 450.f,	TEXT_BASE_POS_Y + 100.f,	150.f,	50.f},	/* 혈액형 */

		XMFLOAT4{TEXT_BASE_POS_X - 60.f,	TEXT_BASE_POS_Y + 200.f,	180.f,	50.f},	/* 색 */
		XMFLOAT4{TEXT_BASE_POS_X + 117.f,	TEXT_BASE_POS_Y + 200.f,	150.f,	50.f},	/* 년 */
		XMFLOAT4{TEXT_BASE_POS_X + 257.5f,	TEXT_BASE_POS_Y + 200.f,	150.f,	50.f},	/* 일 */
		XMFLOAT4{TEXT_BASE_POS_X + 435.f,	TEXT_BASE_POS_Y + 200.f,	180.f,	50.f},	/* 어른 */ // 

		XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y + 300.f,	300.f,	50.f},	/* 관계 */
		XMFLOAT4{TEXT_BASE_POS_X + 350.f,	TEXT_BASE_POS_Y + 300.f,	300.f,	50.f}	/* 작성자 */
	};

	// LayerTag_UI_Text_Button
	{
		CTextButton::TEXTBUTTON_DESC Desc = {};
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		
		for (uint32_t i = 0; i < 11; ++i)
		{
			Desc.fX = PosSizeArr[i].x;
			Desc.fY = PosSizeArr[i].y;
			Desc.fCX = PosSizeArr[i].z;
			Desc.fCY = PosSizeArr[i].w;

			CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
			strgyDesc.ePlaceID = PLACEID::MII_EDITOR;
			strgyDesc.strLayerTag = TEXT("LayerTag_UI_Text_Button");

			Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::MII_EDITOR, TEXT("LayerTag_UI_Text_Button"));
			Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.eTextType = static_cast<CTextButton::TEXTTYPE>(i);

			if (i == 3)
				Desc.pClicked_Strgy_1 = CSetUIisEnable_Strgy::Create(m_pDevice, m_pContext, TRUE);
			else 
				Desc.pClicked_Strgy_1 = CNull_Strgy::Create(m_pDevice, m_pContext);

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID,	TEXT("ProtoTag_UI_TextButton"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,	TEXT("LayerTag_UI_Text_Button"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Text_Button"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}


		// ProtoTag_UI_NoneBack_TextBox
		{
			CNoneBack_TextBox::NONEBACKTEXTBOX_DESC Desc = {};
			Desc.fZ = 0.5f;
			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

			_wstring strArr[] = {
				TEXT("성"),
				TEXT("이름"),
				TEXT("닉네임"),
				TEXT("성별"),
				TEXT("혈액형"),
				TEXT("좋아하는 색"),
				TEXT("생년"),
				TEXT("생월/일"),
				TEXT("어른/아이"),
				TEXT("현실에서 당신과의 관계"),
				TEXT("작성자"),
			};

			for (uint32_t i = 0; i < 11; ++i)
			{
				Desc.fX = PosSizeArr[i].x;
				Desc.fY = PosSizeArr[i].y - 30.f;
				Desc.fCX = PosSizeArr[i].z;
				Desc.fCY = PosSizeArr[i].w;
				Desc.strText = strArr[i];
				Desc.vColor = static_cast<XMFLOAT4>(Colors::DarkSlateGray);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_NoneBack_TextBox"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Text_Button"), tmp)))
					return E_FAIL;
			}
		}

		/* 성별 버튼 눌렀을 때 나오는 Layer */

		/* 배경 */
		// ProtoTag_UI_EditOption_BackGround
		{
			CEditOption_BackGround::EDITOPTIONBACKGROUND_DESC Desc = {};
			Desc.fX = 480.f;
			Desc.fY = 240.f;
			Desc.fZ = 0.5f;
			Desc.fCX = 540.f;
			Desc.fCY = 400.f;
			Desc.fSpeedPerSecond = 10.f;

			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			Desc.TargetFadeTime = 0.3f;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_EditOption_BackGround"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Text_Button"), tmp)))
				return E_FAIL;
		}

		/* 남잔지 여잔지 */
		{
			CCityhallMenu_Button::CITYHALLMENUBUTTON_DESC Desc = {};
			Desc.fCX = 180.f;
			Desc.fCY = 180.f;
			Desc.fZ = 0.5f;
			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			Desc.strSoundTag = TEXT("SFX_Button_Click");

			Desc.isSpecialMove = TRUE;
			Desc.fTargetFadeTime = 0.3f;

			CIconTexture::ICONTEXTURE_DESC IconDesc = {};
			IconDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			IconDesc.fCX = 100.f;
			IconDesc.fCY = 132.f;
			IconDesc.fZ = 0.4f;
			IconDesc.TargetFadeTime = 0.3f;
			IconDesc.isSpecialMove = TRUE;

			uint32_t iIndex = { 0 };

			/* 남자인지 여자인지 */
			for (uint32_t i = 0; i < 2; ++i)
			{
				if (i == 0)
				{
					Desc.fX = 480.f - ((Desc.fCX * 0.5f)) - 30.f;
					Desc.pClicked_Strgy = CSetEditInfo_isMan_Strgy::Create(m_pDevice, m_pContext, TRUE);
				}
				else
				{
					Desc.fX = 480.f + ((Desc.fCX * 0.5f)) + 30.f;
					Desc.pClicked_Strgy = CSetEditInfo_isMan_Strgy::Create(m_pDevice, m_pContext, FALSE);
				}

				Desc.fY = 240.f;
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_CityhallMenu_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Text_Button"), tmp)))
					return E_FAIL;

				/* 아이콘 텍스쳐 */
				IconDesc.fX = Desc.fX;
				IconDesc.fY = Desc.fY;

				IconDesc.iTextureIndex = 6 + iIndex;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Icon"), &IconDesc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Text_Button"), tmp)))
					return E_FAIL;

				++iIndex;
			}
		}

		{
			CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
			Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
			Desc.fCX	= 500.f;
			Desc.fCY	= 50.f;
			Desc.fX		= 480.f;
			Desc.fY		= 410.f;
			Desc.fZ		= 0.3f;
			Desc.strText = L"취소";
			Desc.isSpecialMove = TRUE;
			Desc.TargetFadeTime = 0.3f;
			Desc.strSoundTag = TEXT("SFX_Button_Cancle");

			Desc.pClicked_Strgy			= CSetUIisEnable_Strgy::Create(m_pDevice, m_pContext, FALSE);
			Desc.pSelected_Strgy		= CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy	= CNull_Strgy::Create(m_pDevice, m_pContext);


			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Text_Button"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Text_Button"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}

		/* 혈액형 눌렀을 때 나오는 Layer */
		/* 좋아하는 색 눌렀을 때 나오는 Layer */
		/* 생일 눌렀을 때 나오는 Layer */
	}

	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_Voice_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

	// ProtoTag_UI_Voice_Button
	{
		CVoice_Button::VOICEBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 70.f;
		Desc.fCY = 70.f;
		Desc.fY = 130.f;
		Desc.fZ = 0.5f;

		const float fGap = 20.f;
		const float fPitch = Desc.fCX + fGap;          
		const float fMidX = 480.f;                    
		const float fStartX = fMidX - fPitch * 2.5f;   

		for (uint32_t i = 0; i < 8; ++i)
		{
			Desc.fX = fStartX + fPitch * static_cast<float>(i);
			Desc.iTextureIndex = i;

			Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice,m_pContext, PLACEID::MII_EDITOR, TEXT("LayerTag_UI_Voice_Button"));
			Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			if (i == 6)
			{
				Desc.fX = fStartX;
				Desc.fY = 268.f;
			}
			else if (i == 7)
			{
				Desc.fX = fStartX;
				Desc.fY = 388.f;
			}

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::MII_EDITOR), TEXT("ProtoTag_UI_Voice_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Voice_Button"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Voice_Button"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}
	}


	// ProtoTag_UI_NoneBack_TextBox
	{
		CNoneBack_TextBox::NONEBACKTEXTBOX_DESC Desc = {};
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		XMFLOAT4 PosSizeArr[] = {
			XMFLOAT4{480.f, 40.f, 100.f, 50.f},
			XMFLOAT4{260.f, 80.f, 100.f, 50.f},
			XMFLOAT4{260.f, 220.f, 100.f, 50.f},
			XMFLOAT4{260.f, 340.f, 100.f, 50.f},
		};

		_wstring strArr[] = {
			TEXT("목소리 설정"),
			TEXT("간단 설정"),
			TEXT("랜덤 설정"),
			TEXT("직접 설정"),
		};

		for (uint32_t i = 0; i < 4; ++i)
		{
			Desc.fX = PosSizeArr[i].x;
			Desc.fY = PosSizeArr[i].y;
			Desc.fCX = PosSizeArr[i].z;
			Desc.fCY = PosSizeArr[i].w;
			Desc.strText = strArr[i];
			if(i == 0)
				Desc.vColor = static_cast<XMFLOAT4>(Colors::Black);
			else
				Desc.vColor = static_cast<XMFLOAT4>(Colors::DarkSlateGray);

			
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_NoneBack_TextBox"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Voice_Button"), tmp)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CPlace_MiiEditor::Layer_UI_Personal_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

	//ProtoTag_UI_Personal_Button
	{
		CPersonal_Button::PERSONALBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 40.f;
		Desc.fCY = 40.f;
		Desc.fZ = 0.5f;

		_wstring strArr[] = {
			TEXT("LayerTag_UI_Personal_Action"),
			TEXT("LayerTag_UI_Personal_Accent"),
			TEXT("LayerTag_UI_Personal_Face"),
			TEXT("LayerTag_UI_Personal_Think"),
			TEXT("LayerTag_UI_Personal_Total"),
		};

		XMFLOAT4 vColorArr[] = {
			static_cast<XMFLOAT4>(Colors::Chocolate),
			static_cast<XMFLOAT4>(Colors::DarkOrange),
			static_cast<XMFLOAT4>(Colors::Orange),
			static_cast<XMFLOAT4>(Colors::PeachPuff),

			static_cast<XMFLOAT4>(Colors::PeachPuff),
			static_cast<XMFLOAT4>(Colors::Orange),
			static_cast<XMFLOAT4>(Colors::DarkOrange),
			static_cast<XMFLOAT4>(Colors::Chocolate),
		};
	
		int32_t iPersonalArr[] = {
			-4,-3,-2,-1,1,2,3,4
		};
		const float fGapX = 10.f;
		const float fPitchX = Desc.fCX + fGapX;
		const float fMidX = 500.f;
		const float fStartX = fMidX - fPitchX * 3.5f;

		const float fStartY = 100.f;
		const float fPitchY = 70.f;

		for (uint32_t i = 0; i < 5; ++i)
		{
			for (uint32_t j = 0; j < 8; ++j)
			{
				Desc.fX = fStartX + (fPitchX * j);
				Desc.fY = fStartY + (fPitchY * i);
				Desc.vFixColor = vColorArr[j];

				Desc.iPersonal = iPersonalArr[j];

				Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::MII_EDITOR, strArr[i]);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::MII_EDITOR), TEXT("ProtoTag_UI_Personal_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, strArr[i], tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, strArr[i], static_cast<CButton*>(tmp))))
					return E_FAIL;
			}
		}
	}


	
	// ProtoTag_UI_NoneBack_TextBox
	{
		CNoneBack_TextBox::NONEBACKTEXTBOX_DESC Desc = {};
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		_float fStartY = { 75.f };
		_float fGapY = { 35.f };

		XMFLOAT4 PosSizeArr[] = {
			XMFLOAT4{515.f, fStartY, 100.f, 50.f},
			XMFLOAT4{240.f, fStartY + fGapY, 100.f, 50.f},
			XMFLOAT4{750.f, fStartY + fGapY, 100.f, 50.f},
			XMFLOAT4{515.f, fStartY + fGapY*2, 100.f, 50.f},
			XMFLOAT4{240.f, fStartY + fGapY*3, 100.f, 50.f},
			XMFLOAT4{750.f, fStartY + fGapY*3, 100.f, 50.f},
			XMFLOAT4{515.f, fStartY + fGapY*4, 100.f, 50.f},
			XMFLOAT4{240.f, fStartY + fGapY*5, 100.f, 50.f},
			XMFLOAT4{750.f, fStartY + fGapY*5, 100.f, 50.f},
			XMFLOAT4{515.f, fStartY + fGapY*6, 100.f, 50.f},
			XMFLOAT4{240.f, fStartY + fGapY*7, 100.f, 50.f},
			XMFLOAT4{750.f, fStartY + fGapY*7, 100.f, 50.f},
			XMFLOAT4{515.f, fStartY + fGapY*8, 100.f, 50.f},
			XMFLOAT4{240.f, fStartY + fGapY*9, 100.f, 50.f},
			XMFLOAT4{750.f, fStartY + fGapY*9, 100.f, 50.f},
		};

		_wstring strArr[] = {
			TEXT("행동은..."),
			TEXT("느릿느릿"),
			TEXT("빠릿빠릿"),

			TEXT("말투는..."),
			TEXT("부드럽다"),
			TEXT("직절적이다"),

			TEXT("표정은..."),
			TEXT("굳어 있다"),
			TEXT("풍부하다"),

			TEXT("사고방식은..."),
			TEXT("신중하다"),
			TEXT("낙천적이다"),

			TEXT("한마디로..."),
			TEXT("특이하다"),
			TEXT("상식적이다"),
		};

		for (uint32_t i = 0; i < 15; ++i)
		{
			Desc.fX = PosSizeArr[i].x;
			Desc.fY = PosSizeArr[i].y;
			Desc.fCX = PosSizeArr[i].z;
			Desc.fCY = PosSizeArr[i].w;
			Desc.strText = strArr[i];
			Desc.vColor = static_cast<XMFLOAT4>(Colors::Black);

			
			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_NoneBack_TextBox"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Personal_Action"), tmp)))
				return E_FAIL;
		}
	}
	return S_OK;
}


HRESULT CPlace_MiiEditor::Ready_Environment_Layer()
{
	LIGHT_DESC Desc = {};
	Desc.eType = LIGHT_TYPE::DIRECTIONAL;
	Desc.vDir = _float4(0.5f, 1.f, 4.9f, 0.f);
	Desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.0f);
	Desc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.0f);
	Desc.vSpecular = _float4(1.00f, 0.97f, 0.93f, 1.0f);
	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 0.f, 4.f, -10.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 0.f, 4.f, 0.f, 1.f }, XMConvertToRadians(20.f), 1.f);
	return S_OK;
}


CPlace_MiiEditor* CPlace_MiiEditor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_MiiEditor* pInstance = new CPlace_MiiEditor(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : Place_MiiEditor Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlace_MiiEditor::Free()
{
	CPlace::Free();
}
