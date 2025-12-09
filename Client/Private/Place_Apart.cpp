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

	// ProtoTag_Actor_Player
	{
		CPlayer::ACTOR_DESC tMiiDesc = {};
		tMiiDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		tMiiDesc.fCX = 1.f;
		tMiiDesc.fCY = 1.f;
		tMiiDesc.fCZ = 1.f;
		tMiiDesc.fSpeedPerSecond = 10.f;
		tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tMiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::APART),
			TEXT("ProtoTag_Actor_Player"),
			&tMiiDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::APART),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

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
		uint32_t iNumMii = CApartManager::GetInstance()->Get_ApartInfo().strMiiNames.size();
		for (uint32_t i = 0; i < iNumMii; ++i)
		{
			CSEMii::MII_DESC Desc = {};
			Desc.vPos = XMFLOAT4{ m_pGameInstance->Random(0.f, 10.f) ,0.f, m_pGameInstance->Random(0.f, 10.f) , 1.f };
			Desc.fCX = 1.f;
			Desc.fCY = 1.f;
			Desc.fCZ = 1.f;
			Desc.fSpeedPerSecond = 10.f;
			Desc.fRotationPerSecond = XMConvertToRadians(20.f);
			Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
			_wstring nickname = CApartManager::GetInstance()->Get_ApartInfo().strMiiNames[i];
			_wstring FilePath = L"../../SharedProjects/GAME/GameData/MiiData/";
			FilePath += nickname;
			FilePath += L".json";

			Desc.strSaveDataPath = FilePath;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::STATIC),
				TEXT("ProtoTag_Actor_seMii"),
				&Desc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::STATIC),
				TEXT("LayerTag_Actor"),
				tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}

		}
	}

	// ProtoTag_Actor_Apart
	{
		CApart::APART_DESC tDesc = {};
		tDesc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;
		tDesc.vPos = XMFLOAT4{ 2.f,   3.68115f,  -26.1228f, 1.f };
		tDesc.fCX = 1.f;
		tDesc.fCY = 1.f;
		tDesc.fCZ = 1.f;
		tDesc.fSpeedPerSecond = 10.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::APART),TEXT("ProtoTag_Actor_Apart"),	&tDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::APART), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	/* 타임라인 */
	{
		//CTimeLine_Trans::TIMELINE_TRANS_DESC Desc = {};
		//Desc.pOwner = static_cast<CSEMii*>(tmp);

		//CTimeLine_Trans* pTrans = CTimeLine_Trans::Create(&Desc);

		//CUTFRAME CutFrame = {};
		//CutFrame.vPosition = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		//CutFrame.fTrackPosition = 0.f;
		//pTrans->Add_CuTFrame(CutFrame);

		//CutFrame.vPosition = XMFLOAT4{ 100.f, 100.f, 100.f, 1.f };
		//CutFrame.fTrackPosition = 5.f;
		//pTrans->Add_CuTFrame(CutFrame);

	/* 컷씬 */
		// CCutScene::
	}

	/* 저장 버튼 */
	{
		CCommonButton::COMMONBUTTON_DESC Desc = {};
		Desc.fX = 850.f;
		Desc.fY = 455.f;

		Desc.fCX = 141.f;
		Desc.fCY = 45.f;
		Desc.fSpeedPerSecond = 15.f;

		Desc.pStrategy = CSaveChangePlace_Strategy::Create(m_pDevice, m_pContext);

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

	CWindow_Button::WINDOWBUTTON_DESC tDesc = {};
	tDesc.fCX = 96.f;
	tDesc.fCY = 96.f;
	tDesc.fZ = 0.5f;

	// ProtoTag_UI_WindowButton
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			for (uint32_t j = 0; j < 8; ++j)
			{
				tDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
				tDesc.fX = 137.f + (98.f * j);
				tDesc.fY = 60.f + (100.f * i);
				tDesc.pClicked_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelected_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_WindowButton"), &tDesc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_WindowButton"), tmp)))
					return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT CPlace_Apart::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Apart::Ready_Camera_Layer()
{
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
