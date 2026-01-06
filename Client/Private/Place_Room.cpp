#include "Place_Room.h"          
#include "Place_Loading.h"    
#include "GameInstance.h"        
#include "ApartManager.h"  
#include "ItemManager.h"
#include "MiiManager.h"        

#include "TimeLine_Trans.h"  
#include "CutScene.h"       

/* Component */

/* Actor */
#include "WorldCamera.h" 
#include "TempModel.h"           
#include "UICamera.h"            
#include "Player.h"              
#include "Apart.h"               
#include "SEMii.h"               
#include "Room.h"    

/* UI */
#include "FriendList_BackGround.h"
#include "PlayerInven_Button.h"
#include "Narration_TextBox.h"
#include "YellowBackGround.h"
#include "LevelUp_Inventory.h"
#include "FoodItem_Texture.h"
#include "Category_Button.h"
#include "FoodItem_Button.h"
#include "Thinking_Button.h"
#include "BasicBackGround.h"
#include "Friend_TextBox.h"
#include "BlueBackGround.h"    
#include "Window_Button.h"       
#include "Option_Button.h"
#include "Level_TextBox.h"
#include "Hungry_Gauge.h"
#include "LevelUp_Item.h"
#include "CommonButton.h"   
#include "Back_Button.h"         
#include "IconTexture.h"
#include "HungryBar.h"
#include "LevelBar.h"
#include "TextBox.h"
#include "Gauge.h"

/* Strgy*/
#include "ChangePlace_Strategy.h"
#include "ControlEnable_Strgy.h"
#include "All_Deactive_Strgy.h"
#include "SetFoodIndex_Strgy.h"
#include "Deactive_Strgy.h"
#include "SetisBeFriend_Strgy.h"
#include "SetLove_Strgy.h"
#include "SetLoveMethod_Strgy.h"
#include "SetLovePlace_Strgy.h"

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
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	// ProtoTag_Actor_Room
	{
		CRoom::ROOM_DESC Desc = {};
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f , 1.f };
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iStaticID, TEXT("ProtoTag_Actor_Room"), &Desc);
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
		Desc.iIndex = CMiiManager::GetInstance()->Get_CurMiiIndex();

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

	/* 위쪽 뒷배경 */
	// LayerTag_UI_BackGround
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

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_BlueBackGround_Dot"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_BackGround"), tmp)))
			return E_FAIL;
	}

	/* 친구 목록 뒷배경 */
	// LayerTag_UI_BackGround
	{
		CFriendList_BackGround::FRIENDLISTBACKGROUND_DESC Desc = {};
		Desc.fX		= 480.f;
		Desc.fY		= 240.f;
		Desc.fZ		= 0.1f;
		Desc.fCX	= 960.f;
		Desc.fCY	= 480.f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.strDisableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_BackGround"),
			TEXT("LayerTag_UI_FoodItem"),
			TEXT("LayerTag_UI_LevelUp_Inventory"),
			TEXT("LayerTag_UI_Narration_TextBox"),
			TEXT("LayerTag_UI_Out_Inven_Button"),
			TEXT("LayerTag_UI_TOP"),
		};
		Desc.strEnableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_BackGround"),
			TEXT("LayerTag_UI_FoodItem"),
			TEXT("LayerTag_UI_LevelUp_Inventory"),
			TEXT("LayerTag_UI_Narration_TextBox"),
			TEXT("LayerTag_UI_Out_Inven_Button"),
			TEXT("LayerTag_UI_TOP"),
		};
		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_FriendList_BackGround"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_FriendBackGround"), tmp)))
			return E_FAIL;
	}

	/* 인벤토리 뒷 배경 */
	// LayerTag_UI_PlayerInven_BackGround
	{
		CBlueBackGround::BLUEBACKGORUND_DESC Desc = {};
		Desc.fX		= 480.f;
		Desc.fY		= 240.f;
		Desc.fZ		= 0.6f;
		Desc.fCX	= 960.f;
		Desc.fCY	= 480.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_BlueBackGround_Dot"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_PlayerInven_BackGround"), tmp)))
			return E_FAIL;
	}

	/* 뒤로가기 버튼 */
	// LayerTag_UI_Back_Button
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

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Back_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Back_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	/* 인벤 들어가는 버튼 */
	// LayerTag_UI_Enter_Inventory_Button
	{
		CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 97.f;
		Desc.fCY = 42.f;
		Desc.fX = 60.f;
		Desc.fY = 30.f;
		Desc.fZ = 0.5f;

		CIconTexture::ICONTEXTURE_DESC IconDesc = {};
		IconDesc.eRenderGroup = Desc.eRenderGroup;
		IconDesc.fZ = 0.4f;

		CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
		strgyDesc.ePlaceID = PLACEID::ROOM;

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Enter_Inventory_Button");
		strgyDesc.EnableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_PlayerInven_BackGround"),
			TEXT("LayerTag_UI_Category_Button"),
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_PlayerInven_Food_Main"),
			TEXT("LayerTag_UI_Out_Inven_Button"),
			TEXT("LayerTag_UI_FoodItem"),
			TEXT("LayerTag_UI_GiveItem_Button"),
		};

		strgyDesc.DisableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_Back_Button"),
			TEXT("LayerTag_UI_Enter_Inventory_Button") ,
			TEXT("LayerTag_UI_LevelUp_Inventory") ,
			TEXT("LayerTag_UI_TOP"),
			TEXT("LayerTag_UI_FoodItem_Button"),
			TEXT("LayerTag_UI_Thinking_Button"),
		};

		Desc.pClicked_Strgy			= CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
		Desc.pSelected_Strgy		= CAll_Deactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM);
		Desc.pSelectedClick_Strgy	= CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Enter_Inventory_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Enter_Inventory_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;

		/* 아이콘 텍스쳐 */
		IconDesc.fX = Desc.fX;
		IconDesc.fY = Desc.fY - 4.f;
		IconDesc.iTextureIndex = 21;
		IconDesc.fCX = 39.f;
		IconDesc.fCY = 45.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Icon"), &IconDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Enter_Inventory_Button"), tmp)))
			return E_FAIL;
	}

	/* 인벤에서 나가기 버튼 */
	// LayerTag_UI_Out_Inven_Button
	{
		CBack_Button::BACKBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 100.f;
		Desc.fCY = 68.f;
		Desc.fX = 50.f;
		Desc.fY = 446.f;
		Desc.fZ = 0.5f;

		CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
		strgyDesc.ePlaceID = PLACEID::ROOM;

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Out_Inven_Button");

		strgyDesc.EnableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_Back_Button"),
			TEXT("LayerTag_UI_Enter_Inventory_Button") ,
			TEXT("LayerTag_UI_LevelUp_Inventory"),
			TEXT("LayerTag_UI_TOP"),
			TEXT("LayerTag_UI_Thinking_Button"),
		};

		strgyDesc.DisableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_PlayerInven_BackGround"),
			// 카테고리 버튼
			TEXT("LayerTag_UI_Category_Button"),

			// 서브 카테고리 버튼
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_Sub_Category_Button_Cloth"),

			// 음식 인벤 레이어들
			TEXT("LayerTag_UI_PlayerInven_Food_Main"),
			TEXT("LayerTag_UI_PlayerInven_Food_Side"),
			TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
			TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

			// 옷 인벤 레이어들
			TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),

			// 집/아이템
			TEXT("LayerTag_UI_PlayerInven_House"),
			TEXT("LayerTag_UI_PlayerInven_Item"),
			TEXT("LayerTag_UI_Out_Inven_Button"),

			/* 음식 텍스쳐 */
			TEXT("LayerTag_UI_FoodItem"),

			/* 주기 버튼 */
			TEXT("LayerTag_UI_GiveItem_Button"),
			TEXT("LayerTag_UI_FoodItem_Button"),
		};


		Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Back_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Out_Inven_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Out_Inven_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}

	/* 아이템 텍스쳐 */


	if (FAILED(Layer_UI_LevelUp_Inventory()))
		return E_FAIL;

	if (FAILED(Layer_UI_PlayerInven_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Category_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_SubCategory_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_FoodItem()))
		return E_FAIL;

	if (FAILED(Layer_UI_Favorite_Food_Texture()))
		return E_FAIL;

	if (FAILED(Layer_UI_LevelBar()))
		return E_FAIL;

	if (FAILED(Layer_UI_HungryBar()))
		return E_FAIL;

	if (FAILED(Layer_UI_TextBox()))
		return E_FAIL;

	if (FAILED(Layer_UI_Option_Button()))
		return E_FAIL;

	if (FAILED(Layer_UI_Thinking_Button()))
		return E_FAIL;

	{
		_wstring DisableLayers[] =
		{
			// 배경/공통
			TEXT("LayerTag_UI_PlayerInven_BackGround"),

			// 카테고리 버튼
			TEXT("LayerTag_UI_Category_Button"),

			// 서브 카테고리 버튼
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_Sub_Category_Button_Cloth"),

			// 음식 인벤 레이어들
			TEXT("LayerTag_UI_PlayerInven_Food_Main"),
			TEXT("LayerTag_UI_PlayerInven_Food_Side"),
			TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
			TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

			// 옷 인벤 레이어들
			TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),

			// 집/아이템
			TEXT("LayerTag_UI_PlayerInven_House"),
			TEXT("LayerTag_UI_PlayerInven_Item"),

			TEXT("LayerTag_UI_GiveItem_Button"),
			TEXT("LayerTag_UI_FoodItem_Button"),
			TEXT("LayerTag_UI_BeFriend_Button"),

			TEXT("LayerTag_UI_Love_Button"),
			TEXT("LayerTag_UI_LoveMethod_Button"),
			TEXT("LayerTag_UI_LovePlace_Button"),
		};


		for (_wstring str : DisableLayers)
		{
			m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);
		}

	}
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
		CFoodItem_Texture::FOODITEMTEXTURE_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fCX = 128.f;
		Desc.fCY = 128.f;
		Desc.fX = 480.f;
		Desc.fY = 240.f;
		Desc.fZ = 0.5f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_FoodItem"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_FoodItem"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_UI_FoodItem_Button
	{
		CFoodItem_Button::FOODITEMBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 64.f;
		Desc.fCY = 64.f;
		Desc.fX = 720.f;
		Desc.fY = 120.f;
		Desc.fZ = 0.5f;
		Desc.fSpeedPerSecond = 10.f;

		/* Selected 일 때 FoodItem 비활성화, 파트오브젝트 음식 활성화, */
		Desc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
		strgyDesc.ePlaceID = PLACEID::ROOM;
		strgyDesc.strLayerTag = TEXT("LayerTag_UI_FoodItem_Button");

		strgyDesc.EnableLayers = vector<_wstring>{ TEXT("LayerTag_UI_Enter_Inventory_Button") };
		strgyDesc.DisableLayers = vector<_wstring>{ TEXT("LayerTag_UI_FoodItem_Button") };

		Desc.pSelected_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_FoodItem_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_FoodItem_Button"), tmp)))
			return E_FAIL;
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

	CIconTexture::ICONTEXTURE_DESC IconDesc = {};
	IconDesc.eRenderGroup = Desc.eRenderGroup;
	IconDesc.fZ = 0.4f;

	CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
	strgyDesc.ePlaceID = PLACEID::ROOM;
	strgyDesc.strLayerTag = TEXT("LayerTag_UI_Category_Button");

	vector<_wstring> EnableLayerArr[] = {
		vector<_wstring>{TEXT("LayerTag_UI_Sub_Category_Button_Food"), TEXT("LayerTag_UI_PlayerInven_Food_Main"),},
		vector<_wstring>{TEXT("LayerTag_UI_Sub_Category_Button_Cloth"), TEXT("LayerTag_UI_PlayerInven_Cloth_Man")},
		vector<_wstring>{TEXT("LayerTag_UI_PlayerInven_House")},
		vector<_wstring>{TEXT("LayerTag_UI_PlayerInven_Item")},
	};

	vector<_wstring> DisableLayerArr[] = {
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

	XMFLOAT2 IconSizeArr[] = {
		XMFLOAT2{60.f, 36.f},
		XMFLOAT2{60.f, 36.f},
		XMFLOAT2{60.f, 36.f},
		XMFLOAT2{60.f, 36.f},
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


		/* 아이콘 텍스쳐 */
		IconDesc.fX = Desc.fX;
		IconDesc.fY = Desc.fY;
		IconDesc.iTextureIndex = 8 + i;
		IconDesc.fCX = IconSizeArr[i].x;
		IconDesc.fCY = IconSizeArr[i].y;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Icon"), &IconDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Category_Button"), tmp)))
			return E_FAIL;
	}




	/* 대화 할 때 뜨는 창 */
	{
		CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 264.f;
		Desc.fCY = 60.f;
		Desc.fX = 720.f;
		Desc.fZ = 0.5f;


		for (uint32_t i = 0; i < 2; ++i)
		{
			CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
			strgyDesc.ePlaceID = PLACEID::ROOM;
			strgyDesc.strLayerTag = TEXT("LayerTag_UI_BeFriend_Button");
			strgyDesc.EnableLayers = vector<_wstring>{};
			strgyDesc.DisableLayers = vector<_wstring>{ TEXT("LayerTag_UI_BeFriend_Button"), };
			Desc.pSelected_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			if (i == 0)
			{
				Desc.strText = TEXT("괜찮은데?");
				Desc.pClicked_Strgy = CSetisBeFriend_Strgy::Create(m_pDevice, m_pContext, TRUE);
				Desc.fY = 240.f - (Desc.fCY * 0.6f);
			}
			else
			{
				Desc.strText = TEXT("말리고 싶네");
				Desc.pClicked_Strgy = CSetisBeFriend_Strgy::Create(m_pDevice, m_pContext, FALSE);
				Desc.fY = 240.f + (Desc.fCY * 0.6f);
			}

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_BeFriend_Button"), tmp)))
				return E_FAIL;
		}
	}

	/* 고백할 때 뜨는 창*/
	{
		CCategory_Button::CATEGORYBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 264.f;
		Desc.fCY = 60.f;
		Desc.fX = 720.f;
		Desc.fZ = 0.3f;


		for (uint32_t i = 0; i < 2; ++i)
		{
			CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
			strgyDesc.ePlaceID = PLACEID::ROOM;
			strgyDesc.strLayerTag = TEXT("LayerTag_UI_Love_Button");
			strgyDesc.EnableLayers = vector<_wstring>{};
			strgyDesc.DisableLayers = vector<_wstring>{ TEXT("LayerTag_UI_Love_Button"), };
			Desc.pSelected_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
			Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

			if (i == 0)
			{
				Desc.strText = TEXT("내가 도와줄게");
				Desc.pClicked_Strgy = CSetLove_Strgy::Create(m_pDevice, m_pContext, TRUE);
				Desc.fY = 240.f - (Desc.fCY * 0.6f);
			}
			else
			{
				Desc.strText = TEXT("그건 안 돼!");
				Desc.pClicked_Strgy = CSetLove_Strgy::Create(m_pDevice, m_pContext, FALSE);
				Desc.fY = 240.f + (Desc.fCY * 0.6f);
			}

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Love_Button"), tmp)))
				return E_FAIL;
		}


		/* 고백할 방법 */
		{
			_wstring strTextArr[] = {
				TEXT("무난하게"),
				TEXT("로맨틱하게"),
				TEXT("고자세로"),
				TEXT("저자세로"),
				TEXT("애교스럽게"),
				TEXT("대사를 지정해서"),
				TEXT("노래를 부르면서"),
				TEXT("선물을 주면서"),
			};

			XMFLOAT2 vPosArr[] = {
				XMFLOAT2{480.f - (Desc.fCX*0.6f), 240.f - (Desc.fCY * 1.7f) },
				XMFLOAT2{480.f + (Desc.fCX*0.6f), 240.f - (Desc.fCY * 1.7f) },
				XMFLOAT2{480.f - (Desc.fCX*0.6f), 240.f - (Desc.fCY*0.6f) },
				XMFLOAT2{480.f + (Desc.fCX*0.6f), 240.f - (Desc.fCY*0.6f) },
				XMFLOAT2{480.f - (Desc.fCX*0.6f), 240.f + (Desc.fCY*0.6f) },
				XMFLOAT2{480.f + (Desc.fCX*0.6f), 240.f + (Desc.fCY*0.6f) },
				XMFLOAT2{480.f - (Desc.fCX*0.6f), 240.f + (Desc.fCY * 1.7f) },
				XMFLOAT2{480.f + (Desc.fCX*0.6f), 240.f + (Desc.fCY * 1.7f) },
			};


			for (uint32_t i = 0; i < 8; ++i)
			{
				Desc.fX = vPosArr[i].x;
				Desc.fY = vPosArr[i].y;
				Desc.strText = strTextArr[i];

				CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
				strgyDesc.ePlaceID			= PLACEID::ROOM;
				strgyDesc.strLayerTag		= TEXT("LayerTag_UI_LoveMethod_Button");
				strgyDesc.EnableLayers		= vector<_wstring>{};
				strgyDesc.DisableLayers		= vector<_wstring>{ TEXT("LayerTag_UI_LoveMethod_Button"), };
				Desc.pSelected_Strgy		= CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
				Desc.pSelectedClick_Strgy	= CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pClicked_Strgy			= CSetLoveMethod_Strgy::Create(m_pDevice, m_pContext, i + 1);
				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_LoveMethod_Button"), tmp)))
					return E_FAIL;
			}



			/* 위쪽 뒷배경 */
			// LayerTag_UI_BackGround
			{
				CYellowBackGround::YELLOWBACKGROUND_DESC Desc = {};
				Desc.fX = 480.f;
				Desc.fY = 240.f;
				Desc.fZ = 0.4f;
				Desc.fCX = 960.f;
				Desc.fCY = 480.f;
				Desc.fSpeedPerSecond = 0.f;
				Desc.fRotationPerSecond = XMConvertToRadians(0.f);
				Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_YellowBackGround_Plane"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_LoveMethod_Button"), tmp)))
					return E_FAIL;
			}
		}


		/* 고백할 장소 */
		{
			_wstring strTextArr[] = {
				TEXT("바닷가"),
				TEXT("전망대"),
				TEXT("교실"),
				TEXT("공원"),
				TEXT("음악당"),
				TEXT("찻집"),
			};

			XMFLOAT2 vPosArr[] = {
				XMFLOAT2{480.f - (Desc.fCX * 0.6f), 240.f - (Desc.fCY * 1.3f) },
				XMFLOAT2{480.f + (Desc.fCX * 0.6f), 240.f - (Desc.fCY * 1.3f) },
				XMFLOAT2{480.f - (Desc.fCX * 0.6f), 240.f },
				XMFLOAT2{480.f + (Desc.fCX * 0.6f), 240.f },
				XMFLOAT2{480.f - (Desc.fCX * 0.6f), 240.f + (Desc.fCY * 1.3f) },
				XMFLOAT2{480.f + (Desc.fCX * 0.6f), 240.f + (Desc.fCY * 1.3f) },
			};


			for (uint32_t i = 0; i < 6; ++i)
			{
				Desc.fX = vPosArr[i].x;
				Desc.fY = vPosArr[i].y;
				Desc.strText = strTextArr[i];

				CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
				strgyDesc.ePlaceID = PLACEID::ROOM;
				strgyDesc.strLayerTag = TEXT("LayerTag_UI_LovePlace_Button");
				strgyDesc.EnableLayers = vector<_wstring>{};
				strgyDesc.DisableLayers = vector<_wstring>{ TEXT("LayerTag_UI_LovePlace_Button"), };
				Desc.pSelected_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				Desc.pClicked_Strgy = CSetLovePlace_Strgy::Create(m_pDevice, m_pContext, i + 1);
				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_LovePlace_Button"), tmp)))
					return E_FAIL;
			}



			/* 위쪽 뒷배경 */
			// LayerTag_UI_BackGround
			{
				CYellowBackGround::YELLOWBACKGROUND_DESC Desc = {};
				Desc.fX = 480.f;
				Desc.fY = 240.f;
				Desc.fZ = 0.4f;
				Desc.fCX = 960.f;
				Desc.fCY = 480.f;
				Desc.fSpeedPerSecond = 0.f;
				Desc.fRotationPerSecond = XMConvertToRadians(0.f);
				Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_YellowBackGround_Plane"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_LovePlace_Button"), tmp)))
					return E_FAIL;
			}
		}
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

	CIconTexture::ICONTEXTURE_DESC IconDesc = {};
	IconDesc.eRenderGroup = Desc.eRenderGroup;
	IconDesc.fZ = 0.4f;

	CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
	strgyDesc.ePlaceID = PLACEID::ROOM;

	/* 음식 */
	_wstring LayerTagFoodArr[] = {
		TEXT("LayerTag_UI_PlayerInven_Food_Main"),
		TEXT("LayerTag_UI_PlayerInven_Food_Side"),
		TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
		TEXT("LayerTag_UI_PlayerInven_Food_Drink"),
	};

	XMFLOAT2 FoodIconSizeArr[] = {
		XMFLOAT2{54.f, 18.f},
		XMFLOAT2{54.f, 18.f},
		XMFLOAT2{32.f, 16.f},
		XMFLOAT2{22.f, 18.f},
	};

	for (uint32_t i = 0; i < 4; ++i)
	{
		Desc.fX = 200.f + (100.f * i);

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Sub_Category_Button_Food");
		strgyDesc.EnableLayers = vector<_wstring>{ LayerTagFoodArr[i] };
		strgyDesc.DisableLayers = vector<_wstring>{};
		for (uint32_t j = 0; j < 4; ++j)
		{
			if (j == i)
				continue;

			strgyDesc.DisableLayers.push_back(LayerTagFoodArr[j]);
		}

		Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);

		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Category_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Food"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Food"), static_cast<CButton*>(tmp))))
			return E_FAIL;

		/* 아이콘 텍스쳐 */
		IconDesc.fX = Desc.fX;
		IconDesc.fY = Desc.fY;
		IconDesc.iTextureIndex = 12 + i;
		IconDesc.fCX = FoodIconSizeArr[i].x;
		IconDesc.fCY = FoodIconSizeArr[i].y;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Icon"), &IconDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Food"), tmp)))
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

	XMFLOAT2 ClothIconSizeArr[] = {
		XMFLOAT2{48.f, 20.f},
		XMFLOAT2{48.f, 20.f},
		XMFLOAT2{48.f, 20.f},
		XMFLOAT2{48.f, 20.f},
		XMFLOAT2{48.f, 20.f},
	};

	for (uint32_t i = 0; i < 5; ++i)
	{
		Desc.fX = 200.f + (100.f * i);

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_Sub_Category_Button_Cloth");
		strgyDesc.EnableLayers = vector<_wstring>{ LayerTagClothArr[i] };
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

		/* 아이콘 텍스쳐 */
		IconDesc.fX = Desc.fX;
		IconDesc.fY = Desc.fY;
		IconDesc.iTextureIndex = 16 + i;
		IconDesc.fCX = ClothIconSizeArr[i].x;
		IconDesc.fCY = ClothIconSizeArr[i].y;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Icon"), &IconDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Sub_Category_Button_Cloth"), tmp)))
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

		uint32_t iIndex = {};
		vector<_wstring> MainFoodArr;
		vector<_wstring> MainFoodNumArr;
		vector<uint32_t> MainFoodIdxArr;

		for (uint32_t NumFood : CApartManager::GetInstance()->Get_FoodInventory())
		{
			if (NumFood != 0)
			{
				MainFoodIdxArr.push_back(iIndex);
				MainFoodArr.push_back(CItemManager::GetInstance()->Get_FoodInfo(iIndex).strName);
				MainFoodNumArr.push_back(to_wstring(NumFood));
			}

			++iIndex;
		}

		uint32_t iNumMainFood = static_cast<uint32_t>(MainFoodArr.size());

		uint32_t idx = {};

		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t j = 0; j < 5; ++j)
			{
				Desc.fY = 150.f + 55.f * j;
				Desc.pClicked_Strgy = CDeactive_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM, LayerTagFoodArr[i]);
				Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);



				if (idx < iNumMainFood)
				{
					Desc.strLeftText = MainFoodArr[idx];
					Desc.strRightText = MainFoodNumArr[idx];
					Desc.pSelected_Strgy = CSetFoodIndex_Strgy::Create(m_pDevice, m_pContext, MainFoodIdxArr[idx]);
				}
				else
				{
					Desc.strLeftText = TEXT("");
					Desc.strRightText = TEXT("");
					Desc.pSelected_Strgy = CSetFoodIndex_Strgy::Create(m_pDevice, m_pContext, 0);
				}

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_PlayerInven_Button"), &Desc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, LayerTagFoodArr[i], tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, LayerTagFoodArr[i], static_cast<CButton*>(tmp))))
					return E_FAIL;
				idx++;
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

HRESULT CPlace_Room::Layer_UI_Favorite_Food_Texture()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	{
		// 	CFavoriteFood_Texture::FAVORITEFOODTEXTURE_DESC Desc = {};

		CTextBox::TEXTBOX_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fX = 277.5f;
		Desc.fZ = 0.5f;
		Desc.fCX = 450.f;
		Desc.fCY = 35.f;
		Desc.vTextColor = Colors::AntiqueWhite;
		Desc.strFontTag = TEXT("Basic_Font_16");

		_float yPosArr[] = {
			90.f, 200.f, 350.f
		};

		const _tchar* strTextArr[] = {
			TEXT("가장 좋아하는 음식 / 좋아하는 음식"),
			TEXT("그럭저럭 좋아하는 음식"),
			TEXT("싫어하는 음식 / 가장 싫어하는 음식"),
		};
		for (uint32_t i = 0; i < 3; ++i)
		{
			Desc.strText = strTextArr[i];
			Desc.fY = yPosArr[i];
			Desc.iTextureIndex = 2 + i;
			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_TextBox"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
				return E_FAIL;
		}
	}

	{
		CTextBox::TEXTBOX_DESC Desc = {};

		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fX = 285.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 420.f;
		Desc.fCY = 35.f;
		Desc.iTextureIndex = 1;
		Desc.strFontTag = TEXT("Basic_Font_18");

		_float yPosArr[] = {
			125.f, 165.f, 235.f, 275.f, 315.f, 385.f, 425.f
		};

		for (uint32_t i = 0; i < 7; ++i)
		{
			Desc.fY = yPosArr[i];
			if (i == 0)
			{
				Desc.vTextColor = Colors::Orange;
			}
			else if (i == 6)
			{
				Desc.vTextColor = Colors::CadetBlue;
			}
			else
			{
				Desc.vTextColor = Colors::Black;
			}

			Desc.strText = TEXT("???");

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_TextBox"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_LevelBar()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);
	// ProtoTag_UI_LevelBar
	{
		CLevelBar::LEVELUPBAR_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fX = 700.f;
		Desc.fY = 50.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 320.f;
		Desc.fCY = 40.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_LevelBar"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_UI_LevelGauge 
	{
		CGauge::GAUGE_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fX = 700.f;
		Desc.fY = 50.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 310.f;
		Desc.fCY = 32.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_LevelGauge"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_HungryBar()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	/* ProtoTag_UI_HungryBar */
	{
		CHungryBar::HUNGRYBAR_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fX = 850.f;
		Desc.fY = 150.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 104.f;
		Desc.fCY = 104.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_HungryBar"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
			return E_FAIL;
	}

	/* ProtoTag_UI_HungryGauge */
	{
		CHungryBar::HUNGRYBAR_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fX = 850.f;
		Desc.fY = 150.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 104.f;
		Desc.fCY = 104.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_HungryGauge"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_TextBox()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	{
		CTextBox::TEXTBOX_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		Desc.fZ = 0.5f;
		Desc.iTextureIndex = 0;
		Desc.strFontTag = TEXT("Basic_Font_14");
		Desc.vTextColor = Colors::AntiqueWhite;

		XMFLOAT4 vPosSizeArr[] = {
			XMFLOAT4{300.f, 40.f, 240.f,  28.f},
			XMFLOAT4{700.f, 100.f, 270.f, 28.f},
			XMFLOAT4{760.f, 220.f, 300.f, 28.f},
			XMFLOAT4{760.f, 280.f, 300.f, 28.f},

		};

		const _tchar* strTextArr[] = {
			TEXT("만족도"),
			TEXT("포만도"),
			TEXT("연인"),
			TEXT("절친"),
		};

		for (uint32_t i = 0; i < 4; ++i)
		{
			Desc.fX = vPosSizeArr[i].x;
			Desc.fY = vPosSizeArr[i].y;
			Desc.fCX = vPosSizeArr[i].z;
			Desc.fCY = vPosSizeArr[i].w;
			Desc.strText = strTextArr[i];

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_TextBox"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
				return E_FAIL;
		}
	}

	/* 나레이션 텍스트 박스 */
	// ProtoTag_UI_Narration_TextBox 
	{
		CNarration_TextBox::NARRATION_TEXTBOX_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 400.f;
		Desc.fZ = 0.1f;
		Desc.fCX = 960.f;
		Desc.fCY = 160.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Narration_TextBox"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Narration_TextBox"), tmp)))
			return E_FAIL;
	}

	/* 레벨 */
	// ProtoTag_UI_Level_TextBox
	{
		CLevel_TextBox::LEVELTEXTBOX_DESC Desc = {};
		Desc.fX = 510.f;
		Desc.fY = 80.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 100.f;
		Desc.fCY = 100.f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Level_TextBox"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
			return E_FAIL;
	}

	{
		XMFLOAT4 vPosSizeArr[] = {
			XMFLOAT4{760.f, 250.f, 300.f, 28.f},
			XMFLOAT4{760.f, 310.f, 300.f, 28.f},
		};

		CFriend_TextBox::FRIENDTEXTBOX_DESC Desc = {};
		Desc.fZ = 0.5f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;

		for (uint32_t i = 0; i < 2; ++i)
		{
			Desc.fX = vPosSizeArr[i].x;
			Desc.fY = vPosSizeArr[i].y;
			Desc.fCX = vPosSizeArr[i].z;
			Desc.fCY = vPosSizeArr[i].w;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Friend_TextBox"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_TOP"), tmp)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_Option_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	{
		COption_Button::OPTIONBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 264.f;
		Desc.fCY = 60.f;
		Desc.fX = 960.f - (Desc.fCX * 0.5f) - 10.f;
		Desc.fY = 480.f - (Desc.fCY * 0.5f);
		Desc.fZ = 0.5f;
		Desc.strText = TEXT("주기");


		CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
		strgyDesc.ePlaceID = PLACEID::ROOM;

		strgyDesc.strLayerTag = TEXT("LayerTag_UI_GiveItem_Button");

		strgyDesc.EnableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_Back_Button"),
			TEXT("LayerTag_UI_Enter_Inventory_Button") ,
			TEXT("LayerTag_UI_LevelUp_Inventory"),
			TEXT("LayerTag_UI_TOP"),
			TEXT("LayerTag_UI_FoodItem_Button"),
		};

		strgyDesc.DisableLayers = vector<_wstring>{
			TEXT("LayerTag_UI_PlayerInven_BackGround"),
			// 카테고리 버튼
			TEXT("LayerTag_UI_Category_Button"),

			// 서브 카테고리 버튼
			TEXT("LayerTag_UI_Sub_Category_Button_Food"),
			TEXT("LayerTag_UI_Sub_Category_Button_Cloth"),

			// 음식 인벤 레이어들
			TEXT("LayerTag_UI_PlayerInven_Food_Main"),
			TEXT("LayerTag_UI_PlayerInven_Food_Side"),
			TEXT("LayerTag_UI_PlayerInven_Food_Dessert"),
			TEXT("LayerTag_UI_PlayerInven_Food_Drink"),

			// 옷 인벤 레이어들
			TEXT("LayerTag_UI_PlayerInven_Cloth_Man"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Woman"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Unisex"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Uniform"),
			TEXT("LayerTag_UI_PlayerInven_Cloth_Event"),

			// 집/아이템
			TEXT("LayerTag_UI_PlayerInven_House"),
			TEXT("LayerTag_UI_PlayerInven_Item"),
			TEXT("LayerTag_UI_Out_Inven_Button"),

			/* 음식 텍스쳐 */
			TEXT("LayerTag_UI_FoodItem"),

			/* 주기 버튼 */
			TEXT("LayerTag_UI_GiveItem_Button"),
		};


		Desc.pClicked_Strgy = CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Option_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_GiveItem_Button"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Room::Layer_UI_Thinking_Button()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);


	// ProtoTag_UI_Thinking_Button
	{
		CThinking_Button::THINKINGBUTTON_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fX = 360.f;
		Desc.fY = 120.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 80.f;
		Desc.fCY = 64.f;

		Desc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_UI_Thinking_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Thinking_Button"), tmp)))
			return E_FAIL;
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
