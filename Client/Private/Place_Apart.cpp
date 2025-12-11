#include "Place_Apart.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "TestMii.h"
#include "Player.h"

#include "SEMii.h"
#include "Apart.h"

#include "ApartManager.h"
#include "Room.h"

#include "CutScene.h"
#include "TimeLine_Trans.h"
#include "CommonButton.h"
#include "SaveChangePlace_Strategy.h"

#include "Window_Button.h"
#include "TempModel.h"
#include "ChangePlace_Strategy.h"

#define WINDOW_BASE_POS_X 100.f
#define WINDOW_BASE_POS_Y 200.f

CPlace_Apart::CPlace_Apart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Apart::Initialize()
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

int32_t CPlace_Apart::Update(_float fDT)
{
	int a = 0;
	return 0;
}

HRESULT CPlace_Apart::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("아파트다."));
	return S_OK;
}

HRESULT CPlace_Apart::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("아파트다."));
	return S_OK;
}

HRESULT CPlace_Apart::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::APART);

	// ProtoTag_Actor_Room
	{
		CRoom::ROOM_DESC Desc = {};
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f , 1.f };
		Desc.eRenderGroup = RENDER_GROUP::TOP_BLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Actor_Room"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::APART), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_Actor_seMii
	{
		m_iNumMii = static_cast<uint32_t>(CApartManager::GetInstance()->Get_ApartInfo().strMiiNames.size());
		for (uint32_t i = 0; i < m_iNumMii; ++i)
		{
			CSEMii::MII_DESC Desc = {};
			Desc.vPos = XMFLOAT4{0.f, 0.f, 0.f, 1.f };
			Desc.fCX = 1.5f;
			Desc.fCY = 1.5f;
			Desc.fCZ = 1.5f;
			Desc.fSpeedPerSecond = 10.f;
			Desc.fRotationPerSecond = XMConvertToRadians(20.f);
			Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			_wstring nickname = CApartManager::GetInstance()->Get_ApartInfo().strMiiNames[i];
			_wstring FilePath = L"../../SharedProjects/GAME/GameData/MiiData/" + nickname + L".json";
			Desc.strSaveDataPath = FilePath;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMii"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::APART), TEXT("LayerTag_Actor"), tmp)))
				return E_FAIL;
		}
	}

	// ProtoTag_Actor_Apart
	{
		CApart::APART_DESC tDesc = {};
		tDesc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;
		tDesc.vPos = XMFLOAT4{ 0.f,   5.7f,  -6.2f, 1.f };
		tDesc.fCX = 1.f;
		tDesc.fCY = 1.f;
		tDesc.fCZ = 1.f;
		tDesc.fSpeedPerSecond = 10.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Actor_Apart"), &tDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::APART), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_UI_WindowButton
	{
		CWindow_Button::WINDOWBUTTON_DESC tDesc = {};
		tDesc.fCX = 96.f;
		tDesc.fCY = 96.f;
		tDesc.fZ = 0.5f;

		uint32_t iIndex = { 0 };

		for (int32_t i = 2; i >= 0; --i)
		{
			for (int32_t j = 0; j < 8; ++j)
			{
				tDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
				tDesc.fX = 137.f + (98.f * j);
				tDesc.fY = 60.f + (100.f * i);
				tDesc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelectedClick_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM);

				if (m_iNumMii > iIndex)
					tDesc.isLive = TRUE;
				else
					tDesc.isLive = FALSE;

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_WindowButton"), &tDesc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_WindowButton"), tmp)))
					return E_FAIL;
				++iIndex;
			}
		}
	}


	/* 저장 버튼 */
	{
		CCommonButton::COMMONBUTTON_DESC Desc = {};
		Desc.fX = 850.f;
		Desc.fY = 455.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 141.f;
		Desc.fCY = 45.f;
		Desc.fSpeedPerSecond = 15.f;

		Desc.pStrategy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);

		Desc.eTexturePlaceID = PLACEID::APART;
		Desc.eIconTexturePlaceID = PLACEID::APART;
		Desc.strTextureTag = TEXT("ProtoTag_Component_CMButtonTexture");
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.iTextureIndex = 4;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::APART),
			TEXT("ProtoTag_UI_CMButton"), &Desc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::APART),
			TEXT("LayerTag_Category"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(
			static_cast<uint32_t>(PLACEID::APART), TEXT("LayerTag_Category"),
			static_cast<CButton*>(tmp))))
		{
			return E_FAIL;
		}

	}
	return S_OK;
}

HRESULT CPlace_Apart::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::APART);

	return S_OK;
}

HRESULT CPlace_Apart::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Apart::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 0.f, 7.f, -10.f, 1.f });
	// m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 2.f, 0.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);
	return S_OK;
}

CPlace_Apart* CPlace_Apart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Apart* pPlaceMain = new CPlace_Apart(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Apart::Free()
{
	CPlace::Free();
}
