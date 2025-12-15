#include "ChangePlace_Strategy.h"
#include "ControlEnable_Strgy.h"
#include "PlayerInven_Button.h"
#include "LevelUp_Inventory.h"
#include "Category_Button.h"
#include "TimeLine_Trans.h"      
#include "BlueBackGround.h"    
#include "Deactive_Strgy.h"
#include "Window_Button.h"       
#include "Place_Loading.h"    
#include "GameInstance.h"        
#include "ApartManager.h"  
#include "CommonButton.h"   
#include "LevelUp_Item.h"
#include "WorldCamera.h" 
#include "Back_Button.h"         
#include "Place_Room.h"          
#include "MiiManager.h"        
#include "TempModel.h"           
#include "CutScene.h"            
#include "UICamera.h"            
#include "TestMii.h"             
#include "Player.h"              
#include "Apart.h"               
#include "SEMii.h"               
#include "Room.h"                

#define LEVELUP_INVENTORY_BASE_X 600.f
#define LEVELUP_INVENTORY_BASE_Y 370.f
#define LEVELUP_INVENTORY_GAP_X 90.f
#define LEVELUP_INVENTORY_GAP_Y 70.f

CPlace_Room::CPlace_Room(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Room::Initialize()
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

int32_t CPlace_Room::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_Room::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("방이다."));
	return S_OK;
}

HRESULT CPlace_Room::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("방이다."));
	return S_OK;
}

HRESULT CPlace_Room::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	// ProtoTag_Actor_Room
	{
		CRoom::ROOM_DESC Desc = {};
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f , 1.f };
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_Actor_Room"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_Actor_seMii
	{
		CSEMii::MII_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		Desc.fCX = 1.5f;
		Desc.fCY = 1.5f;
		Desc.fCZ = 1.5f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.fRotationPerSecond = XMConvertToRadians(20.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;
		Desc.strNickName = CMiiManager::GetInstance()->Get_CurMiiNickName();

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMii"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Room::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	/* 뒷배경 */
	// ProtoTag_UI_BlueBackGround
	{
		CBlueBackGround::BLUEBACKGORUND_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 240.f;
		Desc.fZ = 1.f;
		Desc.fCX = 960.f;
		Desc.fCY = 480.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_BlueBackGround"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,TEXT("LayerTag_UI"), tmp)))
			return E_FAIL;
	}

	/* 뒤로가기 버튼 */
	// ProtoTag_UI_Back_Button
	{
		CBack_Button::BACKBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 100.f;
		Desc.fCY = 68.f;
		Desc.fX = 50.f;
		Desc.fY = 446.f;
		Desc.fZ = 0.5f;
		Desc.pClicked_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::APART);
		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
	
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Back_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI"), tmp)))
			return E_FAIL;
	}

	if (FAILED(Layer_UI_LevelUp_Inventory()))
		return E_FAIL;

	if (FAILED(Layer_UI_PlayerInven_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Category_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_SubCategory_Button()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlace_Room::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Room::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 0.f, 5.f, -15.f, 1.f });
	// m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 2.f, 0.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);
	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_LevelUp_Inventory()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	/* 레벨업 선물 인벤토리 */
	// ProtoTag_UI_LevelUp_Inventory
	{
		CLevelUp_Inventory::LEVELUPINVENTORY_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fZ = 0.5f;
		Desc.fCX = 80.f;
		Desc.fCY = 69.f;

		uint32_t iIndex = { 0 };

		for (uint32_t i = 0; i < 2; ++i)
		{
			for (uint32_t j = 0; j < 4; ++j)
			{
				Desc.fX = LEVELUP_INVENTORY_BASE_X + (LEVELUP_INVENTORY_GAP_X * j);
				Desc.fY = LEVELUP_INVENTORY_BASE_Y + (LEVELUP_INVENTORY_GAP_Y * i);
				Desc.iInventoryIndex = iIndex;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_LevelUp_Inventory"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_LevelUp_Inventory"), tmp)))
					return E_FAIL;

				++iIndex;
			}
		}
		
	}
	return S_OK;
}

HRESULT CPlace_Room::Layer_Actor_LevelUp_Item()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);



	return S_OK;
}

/* ProtoTag_UI_FoodItem */
HRESULT CPlace_Room::Layer_UI_FoodItem()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	{
		for (uint32_t i = 0; i < 25; ++i)
		{

		}
	}
	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_Category_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
	Desc.fCX = 120.f;
	Desc.fCY = 52.f;
	Desc.fY = 40.f;
	Desc.fZ = 0.5f;

	CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
	strgyDesc.ePlaceID = PLACEID::ROOM;
	strgyDesc.strLayerTag = TEXT("LayerTag_UI_Category_Button");

	vector<_wstring> EnableLayerArr[] = {
		vector<_wstring>{TEXT("LayerTag_UI_Sub_Category_Button_Food"), TEXT("LayerTag_UI_PlayerInven_Food_Main"),},
		vector<_wstring>{TEXT("LayerTag_UI_Sub_Category_Button_Cloth"), TEXT("LayerTag_UI_PlayerInven_Cloth_Man")},
		vector<_wstring>{TEXT("LayerTag_UI_PlayerInven_House")},
		vector<_wstring>{TEXT("LayerTag_UI_PlayerInven_Item")},
	};

	vector<_wstring> DisableLayerArr[] =
	{
		// [0] FOOD 버튼 클릭 시: "Food_Main"만 켜고 나머지는 전부 끄기
		vector<_wstring>{
			TEXT("LayerTag_UI_Sub_Category_Button_Cloth"),
			TEXT("LayerTag_UI_PlayerInven_House"),
			TEXT("LayerTag_UI_PlayerInven_Item"),

				// Food에서 기본(Main) 제외 나머지 꺼두기
				TEXT("LayerTag_UI_PlayerInven_Food_Side"),
				TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
				TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

				// Cloth 전부 끄기
				TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),
			},

		// [1] CLOTH 버튼 클릭 시: "Cloth_Man"만 켜고 나머지는 전부 끄기
		vector<_wstring>{
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_PlayerInven_House"),
			TEXT("LayerTag_UI_PlayerInven_Item"),

				// Food 전부 끄기
				TEXT("LayerTag_UI_PlayerInven_Food_Main"),
				TEXT("LayerTag_UI_PlayerInven_Food_Side"),
				TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
				TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

				// Cloth에서 기본(Man) 제외 나머지 꺼두기
				TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),
			},

		// [2] HOUSE 버튼 클릭 시: House만 켜고 나머지 전부 끄기
		vector<_wstring>{
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_Sub_Category_Button_Cloth"),
			TEXT("LayerTag_UI_PlayerInven_Item"),

				// Food 전부 끄기
				TEXT("LayerTag_UI_PlayerInven_Food_Main"),
				TEXT("LayerTag_UI_PlayerInven_Food_Side"),
				TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
				TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

				// Cloth 전부 끄기
				TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),
			},

		// [3] ITEM 버튼 클릭 시: Item만 켜고 나머지 전부 끄기
		vector<_wstring>{
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_Sub_Category_Button_Cloth"),
			TEXT("LayerTag_UI_PlayerInven_House"),

				// Food 전부 끄기
				TEXT("LayerTag_UI_PlayerInven_Food_Main"),
				TEXT("LayerTag_UI_PlayerInven_Food_Side"),
				TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
				TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

				// Cloth 전부 끄기
				TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
				TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),
			},
	};

	for (uint32_t i = 0; i < 4; ++i)
	{
		Desc.fX = 200.f + (125.f * i);
		strgyDesc.DisableLayers = DisableLayerArr[i];
		strgyDesc.EnableLayers = EnableLayerArr[i];
		Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Category_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Category_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_SubCategory_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
	Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
	Desc.fCX = 97.f;
	Desc.fCY = 42.f;
	Desc.fY = 100.f;
	Desc.fZ = 0.5f;

	CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
	strgyDesc.ePlaceID = PLACEID::ROOM;

	/* 음식 */
	_wstring LayerTagFoodArr[] = {
		TEXT("LayerTag_UI_PlayerInven_Food_Main"),
		TEXT("LayerTag_UI_PlayerInven_Food_Side"),
		TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
		TEXT("LayerTag_UI_PlayerInven_Food_Drink"),
	};

	for (uint32_t i = 0; i < 4; ++i)
	{
		Desc.fX						= 200.f + (100.f * i);

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Sub_Category_Button_Food");
		strgyDesc.EnableLayers = vector<_wstring>{ LayerTagFoodArr[i] };
		strgyDesc.DisableLayers = vector<_wstring>{}; 
		for (uint32_t j = 0; j < 4; ++j)
		{
			if (j == i)
				continue;

			strgyDesc.DisableLayers.push_back(LayerTagFoodArr[j]);
		}

		Desc.pClicked_Strgy			= CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);

		Desc.pSelected_Strgy		= CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy	= CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Food"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Food"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	/* 옷 */
	_wstring LayerTagClothArr[] = {
		TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
		TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
		TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
		TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
		TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),
	};

	for (uint32_t i = 0; i < 5; ++i)
	{
		Desc.fX = 200.f + (100.f * i);

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Sub_Category_Button_Cloth");
		strgyDesc.EnableLayers = vector<_wstring>{ LayerTagClothArr[i]};
		strgyDesc.DisableLayers = vector<_wstring>{};
		for (uint32_t j = 0; j < 5; ++j)
		{
			if (i == j)
				continue;
			strgyDesc.DisableLayers.push_back(LayerTagClothArr[j]);
		}

		Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);

		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Cloth"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Cloth"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_PlayerInven_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	//LayerTag_UI_PlayerInven_Button
	{
		CPlayerInven_Button::PLAYERINVENBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fX = 480.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 700.f;
		Desc.fCY = 50.f;

		/* 음식 */
		_wstring LayerTagFoodArr[] = {
			TEXT("LayerTag_UI_PlayerInven_Food_Main"),
			TEXT("LayerTag_UI_PlayerInven_Food_Side"),
			TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
			TEXT("LayerTag_UI_PlayerInven_Food_Drink"),
		};

		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 5; ++j)
			{
				Desc.fY = 150.f + 55.f * j;
				Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM, LayerTagFoodArr[i]);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_PlayerInven_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, LayerTagFoodArr[i], tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, LayerTagFoodArr[i], static_cast<CButton*>(tmp))))
					return E_FAIL;
			}
		}

		/* 옷 */
		_wstring LayerTagClothArr[] = {
			TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),
		};

		for (uint32_t i = 0; i < 5; ++i)
		{
			for (uint32_t j = 0; j < 5; ++j)
			{
				Desc.fY = 150.f + 55.f * j;
				Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM, LayerTagClothArr[i]);
				Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_PlayerInven_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, LayerTagClothArr[i], tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, LayerTagClothArr[i], static_cast<CButton*>(tmp))))
					return E_FAIL;
			}
		}

		/* 집 */
		for (uint32_t i = 0; i < 5; ++i)
		{
			Desc.fY = 150.f + 55.f * i;
			Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM, TEXT("LayerTag_UI_PlayerInven_House"));
			Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_PlayerInven_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_PlayerInven_House"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_PlayerInven_House"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}

		/* 아이템 */
		for (uint32_t i = 0; i < 5; ++i)
		{
			Desc.fY = 150.f + 55.f * i;
			Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM, TEXT("LayerTag_UI_PlayerInven_Item"));
			Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_PlayerInven_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_PlayerInven_Item"), tmp)))
				return E_FAIL;

			if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_PlayerInven_Item"), static_cast<CButton*>(tmp))))
				return E_FAIL;
		}
	}

	return S_OK;
}

CPlace_Room* CPlace_Room::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Room* pPlaceMain = new CPlace_Room(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Room::Free()
{
	CPlace::Free();
}
