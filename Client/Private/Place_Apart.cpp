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

#include "Window_Button.h"
#include "TempModel.h"
#include "ChangePlace_Strategy.h"
#include "ControlEnable_Strgy.h"

#include "MiiManager.h"	

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
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_Room"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	// ProtoTag_Actor_seMii
	{
		m_iNumMii = CApartManager::GetInstance()->Get_ApartInfo_NumMii();

		for (uint32_t i = 0; i < m_iNumMii; ++i)
		{
			CMiiManager::GetInstance()->Set_CurMiiIndex(i);

			CSEMii::MII_DESC Desc = {};
			Desc.vPos = XMFLOAT4{0.f, 0.f, 2.f, 1.f };
			Desc.fCX = 1.5f;
			Desc.fCY = 1.5f;
			Desc.fCZ = 1.5f;
			Desc.fSpeedPerSecond = 10.f;
			Desc.fRotationPerSecond = XMConvertToRadians(20.f);
			Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			_wstring nickname = CApartManager::GetInstance()->Get_ApartInfo_MiiName(i);
			Desc.strNickName = nickname;
			Desc.iIndex = i;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMii"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			CMiiManager::GetInstance()->Get_MiiInfo(i).MiiProfile.strNickName;

			_wstring strLayerTag = L"LayerTag_Actor_" + nickname;
			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::APART), strLayerTag, tmp)))
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

	
	vector<_wstring> AllMiiNameArr;
	vector<_wstring> AllMiiLayers;

	// ProtoTag_UI_WindowButton
	{
		CWindow_Button::WINDOWBUTTON_DESC tDesc = {};
		tDesc.fCX = 96.f;
		tDesc.fCY = 96.f;
		tDesc.fZ = 0.5f;

		uint32_t iIndex = { 0 };

		/* 주민들의 이름 */

		uint32_t iNumMii = CApartManager::GetInstance()->Get_ApartInfo_NumMii();
		AllMiiNameArr.reserve(iNumMii);

		for (uint32_t i = 0; i < iNumMii; ++i)
		{
			_wstring Name = CApartManager::GetInstance()->Get_ApartInfo_MiiName(i);
			AllMiiNameArr.push_back(Name);

			AllMiiLayers.push_back(L"LayerTag_Actor_" + Name);
		}

		for (int32_t i = 2; i >= 0; --i)
		{
			for (int32_t j = 0; j < 8; ++j)
			{
				tDesc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
				tDesc.fX = 137.f + (98.f * j);
				tDesc.fY = 60.f + (100.f * i);

				CControlEnable_Strgy::CONTROLENABLESTRGY_DESC strgyDesc = {};
				strgyDesc.ePlaceID = PLACEID::APART;

				vector<_wstring> EnableLayer;
				vector<_wstring> DisableLayer;
				_wstring strLayerTag = {};

				uint32_t iLayerIndex = { 0 };

				for (auto& str : AllMiiNameArr)
				{
					strLayerTag = L"LayerTag_Actor_" + str;
					if (iLayerIndex == iIndex)
					{
						tDesc.strMiiNickName = str;
						EnableLayer.push_back(strLayerTag);
					}
					else
						DisableLayer.push_back(strLayerTag);

					++iLayerIndex;
				}

				strgyDesc.EnableLayers = EnableLayer;
				strgyDesc.DisableLayers = DisableLayer;
				strgyDesc.strLayerTag = strLayerTag;

				tDesc.pClicked_Strgy	= CControlEnable_Strgy::Create(m_pDevice, m_pContext, strgyDesc);
				tDesc.pSelected_Strgy	= CNull_Strgy::Create(m_pDevice, m_pContext);
				tDesc.pSelectedClick_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ROOM);
				
				/* 인덱스로 보관 */

				if (m_iNumMii > iIndex)
				{
					tDesc.iMiiIndex = iIndex;
					tDesc.isLive = TRUE;

				}
				else
				{
					tDesc.iMiiIndex = 0;
					tDesc.isLive = FALSE;
				}

				tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_WindowButton"), &tDesc);
				if (nullptr == tmp)
					return E_FAIL;

				if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_WindowButton"), tmp)))
					return E_FAIL;

				if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, strLayerTag, dynamic_cast<CButton*>(tmp))))
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

	for (const auto& str : AllMiiLayers)
	{
		m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);
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
	LIGHT_DESC Desc = {};
	Desc.eType = LIGHT_TYPE::DIRECTIONAL;
	Desc.vDir = _float4(1.5f, -1.f, 3.5f, 0.f);

	Desc.vDiffuse = _float4(0.55f, 0.62f, 0.80f, 1.0f);
	Desc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.0f); // 그림자(비조명) 영역 들어올림
	Desc.vSpecular = _float4(0.85f, 0.90f, 0.88f, 1.0f); // 과한 번쩍임만 살짝 완화

	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;
	

	Desc.eType = LIGHT_TYPE::POINT;
	Desc.vPos = _float4(0.f, 18.f, -20.f, 1.f);
	Desc.fRange = 42.f;
	Desc.vDiffuse = _float4(1.00f, 0.96f, 0.90f, 1.0f);
	Desc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	Desc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

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
