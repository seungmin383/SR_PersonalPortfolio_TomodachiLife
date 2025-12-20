#include "Place_FoodStore.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "ItemManager.h"

/* Actor */
#include "Store_Food.h"
#include "SEMii.h"

/* UI */
#include "Category_Button.h"
#include "IconTexture.h"
#include "YellowBackGround.h"
#include "BasicBackGround.h"
#include "PlayerInven_Button.h"
#include "Back_Button.h"
#include "Option_Button.h"
#include "Wallet.h"
#include "FoodItem_Texture.h"

/* strgy */
#include "ChangePlace_Strategy.h"
#include "Deactive_Strgy.h"
#include "SetFoodIndex_Strgy.h"
#include "BuyFood_Strgy.h"

CPlace_FoodStore::CPlace_FoodStore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_FoodStore::Initialize()
{
	if (FAILED(Ready_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Camera_Layer()))
		return E_FAIL;

	CItemManager::GetInstance()->Set_ItemIndex(ITEM_TYPE::FOOD, 0);

	return S_OK;
}

int32_t CPlace_FoodStore::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_FoodStore::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("음식 상점이다."));
	return S_OK;
}

HRESULT CPlace_FoodStore::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("음식 상점이다."));
	return S_OK;
}

HRESULT CPlace_FoodStore::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	/* 식품점 */
	// 
	{
		CStore_Food::STORE_FOOD_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		Desc.fCX = 0.03f;
		Desc.fCY = 0.03f;
		Desc.fCZ = 0.03f;
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_Actor_FoodStore"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;	

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_FoodStore::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);
	
	if (FAILED(Layer_UI_Category_Button()))
		return E_FAIL;	
	if (FAILED(Layer_UI_Bottom_Background()))
		return E_FAIL;	
	if (FAILED(Layer_UI_FoodList_Button()))
		return E_FAIL;	
	if (FAILED(Layer_UI_BackButton()))
		return E_FAIL;	
	if (FAILED(Layer_UI_Option_Button()))
		return E_FAIL;	
	if (FAILED(Layer_UI_Wallet()))
		return E_FAIL;	
	if (FAILED(Layer_UI_FoodItem()))
		return E_FAIL;	
	return S_OK;
}

HRESULT CPlace_FoodStore::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_FoodStore::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 0.f, 4.f, 10.f, 1.f }, XMConvertToRadians(13.f), 1.f);
	return S_OK;
}

HRESULT CPlace_FoodStore::Layer_UI_Bottom_Background()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 노랑 땡땡이 백그라운드 */
	{
		CYellowBackGround::YELLOWBACKGROUND_DESC Desc = {};
		Desc.fX		= 480.f;
		Desc.fY		= 240.f;
		Desc.fZ		= 1.f;
		Desc.fCX	= 960.f;
		Desc.fCY	= 480.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iStaticID, TEXT("ProtoTag_UI_YellowBackGround_Dot"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,	TEXT("LayerTag_UI_Bottom_Background"), tmp)))
			return E_FAIL;
	}

	/* 노랑 백그라운드 */
	// ProtoTag_UI_YellowBackGround_Plane
	{
		CBasicBackGround::BASICBACKGROUND_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 280.f;
		Desc.fZ = 0.9f;
		Desc.fCX = 960.f;
		Desc.fCY = 400.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iStaticID, TEXT("ProtoTag_UI_YellowBackGround_Plane"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Bottom_Background"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CPlace_FoodStore::Layer_UI_Category_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
	Desc.fCX	= 360.f;
	Desc.fCY	= 70.f;
	Desc.fX		= 295.f;
	Desc.fY		= 45.f;
	Desc.fZ		= 0.5f;
	Desc.strText = L"오늘의 상품";

	Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::FOOD_STORE, TEXT("LayerTag_UI_Category_Button"));
	Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
	Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);


	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Category_Button"), tmp)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Category_Button"), static_cast<CButton*>(tmp))))
		return E_FAIL;

	Desc.fX = 665.f;
	Desc.strText = L"카탈로그";

	Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::FOOD_STORE, TEXT("LayerTag_UI_Category_Button"));
	Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
	Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

	tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Category_Button"), tmp)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Category_Button"), static_cast<CButton*>(tmp))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlace_FoodStore::Layer_UI_FoodList_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	{
		CPlayerInven_Button::PLAYERINVENBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fX = 480.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 700.f;
		Desc.fCY = 50.f;


		/* 랜덤 인덱스를 받아올 배열 */
		uint32_t iRandFoodArr[] = {
			0,0,0,0,0
		};

		_bool isSame = { FALSE };

		/* 겹치지 않는 5개의 인덱스를 저장 */
		for (uint32_t i = 0; i < 5;)
		{
			isSame = FALSE;
			uint32_t x = rand() % CItemManager::GetInstance()->Get_NumFood() + 1;

			for (uint32_t j = 0; j < 5; ++j)
			{
				if (x == iRandFoodArr[j])
				{
					isSame = TRUE;
					break;
				}
			}

			if (!isSame)
			{
				iRandFoodArr[i] = x;
				++i;
			}
		}
		

		vector<_wstring> vecFoodNameArr;
		vector<uint32_t> vecFoodPriceArr;

		/* 인덱스에 맞는 음식을 꺼내옴 */
		for (uint32_t i = 0; i < 5; ++i)
		{
			uint32_t index = iRandFoodArr[i];

			_wstring FoodName = CItemManager::GetInstance()->Get_FoodInfo(index).strName;
			uint32_t iPrice = CItemManager::GetInstance()->Get_FoodInfo(index).iPrice;

			vecFoodNameArr.push_back(FoodName);
			vecFoodPriceArr.push_back(iPrice);
		}


		for (uint32_t i = 0; i < 5; ++i)
		{
			Desc.fY = 150.f + 55.f * i;

			Desc.strLeftText = vecFoodNameArr[i];
			Desc.strRightText = to_wstring(vecFoodPriceArr[i]) + L" 원";
				
			Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::FOOD_STORE, TEXT("LayerTag_UI_ItemList_Button"));
			Desc.pSelected_Strgy = CSetFoodIndex_Strgy::Create(m_pDevice, m_pContext, iRandFoodArr[i]);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_PlayerInven_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_ItemList_Button"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_ItemList_Button"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}
	}

	return S_OK;	
}

HRESULT CPlace_FoodStore::Layer_UI_BackButton()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	/* 뒤로가기 버튼 */
	// LayerTag_UI_Back_Button
	{
		CBack_Button::BACKBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX	= 100.f;
		Desc.fCY	= 68.f;
		Desc.fX		= 50.f;
		Desc.fY		= 446.f;
		Desc.fZ		= 0.5f;
		Desc.pClicked_Strgy = CSetFoodIndex_Strgy::Create(m_pDevice, m_pContext, 0);
		Desc.pSelected_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
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
}

HRESULT CPlace_FoodStore::Layer_UI_Option_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	{
		COption_Button::OPTIONBUTTON_DESC Desc = {};
		Desc.fX = 850.f;
		Desc.fY = 455.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 220.f;
		Desc.fCY = 68.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.strText = TEXT("구입하기");
		Desc.pClicked_Strgy = CBuyFood_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelected_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::FOOD_STORE, TEXT("LayerTag_UI_Buy_Button"));
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Option_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Buy_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Buy_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;

	}
	return S_OK;
}

HRESULT CPlace_FoodStore::Layer_UI_Wallet()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	// ProtoTag_UI_Wallet
	{
		CWallet::WALLET_DESC Desc = {};
		Desc.fX		= 550.f;
		Desc.fY		= 450.f;
		Desc.fZ		= 0.5f;
		Desc.fCX	= 320.f;
		Desc.fCY	= 48.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.pUpdateStrgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Wallet"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Wallet"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_FoodStore::Layer_UI_FoodItem()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::FOOD_STORE);

	{
		CFoodItem_Texture::FOODITEMTEXTURE_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fCX = 128.f;
		Desc.fCY = 128.f;
		Desc.fX = 600.f;
		Desc.fY = 240.f;
		Desc.fZ = 0.5f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_FoodItem"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Wallet"), tmp)))
			return E_FAIL;
	}

	return S_OK;	
}

CPlace_FoodStore* CPlace_FoodStore::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_FoodStore* pPlaceMain = new CPlace_FoodStore(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_FoodStore::Free()
{
	CPlace::Free();
}
