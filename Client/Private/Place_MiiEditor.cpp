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

	// ProtoTag_Actor_Mii
	{
		CMii::MII_DESC MiiDesc = {};
		MiiDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0, 1.f };
		MiiDesc.fCX = 1.f;
		MiiDesc.fCY = 1.f;
		MiiDesc.fCZ = 1.f;
		MiiDesc.fSpeedPerSecond = 0.f;
		MiiDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiDesc.fRotationPerSecond = XMConvertToRadians(90.f);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC),
			TEXT("ProtoTag_Actor_Mii"),	&MiiDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::MII_EDITOR),
			TEXT("LayerTag_Actor"),	tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_UI_Layer()
{
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

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
		TEXT("LayerTag_UI_Text_Button")
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
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

	CPageButton::PAGEBUTTON_DESC Desc = {};
	Desc.fY = CATEGORY_BASE_POS_Y;
	Desc.fZ = 0.5f;
	Desc.fCX = 93.f;
	Desc.fCY = 45.f;
	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

	vector<_wstring> LayerTagArr{
		TEXT("LayerTag_UI_Page_Button"),
		TEXT("LayerTag_UI_FaceShape"),
		TEXT("LayerTag_UI_Hair"),
		TEXT("LayerTag_UI_Eyebrow"),
		TEXT("LayerTag_UI_Eyes"),
		TEXT("LayerTag_UI_Nose"),
		TEXT("LayerTag_UI_Mouth"),
		TEXT("LayerTag_UI_Text_Button")
	};

	vector<uint32_t> EnableLayerIndex[] = {
		vector<uint32_t>{0, 1},
		vector<uint32_t>{7},
		vector<uint32_t>{},
		vector<uint32_t>{}
	};

	vector<uint32_t> DisableLayerIndex[] = {
			vector<uint32_t>{2,3,4,5,6,7},
			vector<uint32_t>{0,1,2,3,4,5,6},
			vector<uint32_t>{0,1,2,3,4,5,6,7},
			vector<uint32_t>{0,1,2,3,4,5,6,7}
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
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::MII_EDITOR);

	//LayerTag_UI_Save_Button
	{
		COption_Button::OPTIONBUTTON_DESC Desc = {};
		Desc.fX = 850.f;
		Desc.fY = 455.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 141.f;
		Desc.fCY = 45.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		/* 이거 삭제하고 End_Edit이나 BeginEdit에서 컨트롤 하게 해야겠다 */
		Desc.pClicked_Strgy = CSaveMiiData_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelected_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
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
	// LayerTag_UI_Text_Button
	{
		CTextButton::TEXTBUTTON_DESC Desc = {};
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		XMFLOAT4 PosSizeArr[] = {
			XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y,			200.f,	50.f},	/* 성 */
			XMFLOAT4{TEXT_BASE_POS_X + 300.f,	TEXT_BASE_POS_Y,			200.f,	50.f},	/* 이름 */
			XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y + 80.f,		200.f,	50.f},	/* 닉네임 */
			XMFLOAT4{TEXT_BASE_POS_X + 250.f,	TEXT_BASE_POS_Y + 80.f,		80.f,	50.f},	/* 성별 */
			XMFLOAT4{TEXT_BASE_POS_X + 350.f,	TEXT_BASE_POS_Y + 80.f,		66.f,	50.f},	/* 혈액형 */
			XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y + 160.f,	66.f,	50.f},	/* 색 */
			XMFLOAT4{TEXT_BASE_POS_X + 100.f,	TEXT_BASE_POS_Y + 160.f,	80.f,	50.f},	/* 년 */
			XMFLOAT4{TEXT_BASE_POS_X + 180.f,	TEXT_BASE_POS_Y + 160.f,	80.f,	50.f},	/* 일 */
			XMFLOAT4{TEXT_BASE_POS_X + 250.f,	TEXT_BASE_POS_Y + 160.f,	100.f,	50.f},	/* 어른 */
			XMFLOAT4{TEXT_BASE_POS_X,			TEXT_BASE_POS_Y + 240.f,	200.f,	50.f},	/* 관계 */
			XMFLOAT4{TEXT_BASE_POS_X + 300.f,	TEXT_BASE_POS_Y + 240.f,	200.f,	50.f}	/* 작성자 */
		};
		
		for (uint32_t i = 0; i < 11; ++i)
		{
			Desc.fX = PosSizeArr[i].x;
			Desc.fY = PosSizeArr[i].y;
			Desc.fCX = PosSizeArr[i].z;
			Desc.fCY = PosSizeArr[i].w;

			Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::MII_EDITOR, TEXT("LayerTag_UI_Text_Button"));
			Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.eTextType = static_cast<CTextButton::TEXTTYPE>(i);

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID,	TEXT("ProtoTag_UI_TextButton"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,	TEXT("LayerTag_UI_Text_Button"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Text_Button"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}

	}
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_Environment_Layer()
{
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
