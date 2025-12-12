#include "ChangePlace_Strategy.h"
#include "LevelUp_Inventory.h"
#include "TimeLine_Trans.h"      
#include "BlueBackGround.h"      
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
