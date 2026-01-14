#include "Place_Event_BeFriend.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

/* Manager */
#include "MiiManager.h"
#include "ApartManager.h"
#include "GameDirector.h"
/* Actor */
#include "Room.h"
#include "SEMii.h"

/* UI */
#include "BlueBackGround.h"

CPlace_Event_BeFriend::CPlace_Event_BeFriend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Event_BeFriend::Initialize()
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

int32_t CPlace_Event_BeFriend::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_Event_BeFriend::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("친구가 돼."));
	return S_OK;
}

HRESULT CPlace_Event_BeFriend::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("친구가 돼."));
	return S_OK;
}

HRESULT CPlace_Event_BeFriend::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_BEFRIEND);
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	// ProtoTag_Actor_Room
	{
		CRoom::ROOM_DESC Desc = {};
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.fSpeedPerSecond = 10.f;
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f , 1.f };
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

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
		Desc.fCX = 1.5f;
		Desc.fCY = 1.5f;
		Desc.fCZ = 1.5f;
		Desc.fSpeedPerSecond = 1.f;
		Desc.fRotationPerSecond = XMConvertToRadians(20.f);
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		Desc.strNickName = CMiiManager::GetInstance()->Get_CurMiiNickName();
		for (uint32_t i = 0; i < 2; ++i)
		{
			if (i == 0)
			{
				Desc.vPos = XMFLOAT4{ 0.f, 0.f, -3.f, 1.f };
				Desc.iIndex = CApartManager::GetInstance()->Get_SrcMiiIndex();
			}
			else
			{
				Desc.vPos = XMFLOAT4{ 0.f, 0.f, 10.f, 1.f };
				Desc.iIndex = CApartManager::GetInstance()->Get_DstMiiIndex();

			}

			tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMii"), &Desc);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CPlace_Event_BeFriend::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_BEFRIEND);

	{
		CBlueBackGround::BLUEBACKGORUND_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 240.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 960.f;
		Desc.fCY = 480.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iStaticID, TEXT("ProtoTag_UI_BlueBackGround_Dot"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::EVENT_BEFRIEND),
			TEXT("LayerTag_UI"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}	
	}
	return S_OK;
}

HRESULT CPlace_Event_BeFriend::Ready_Environment_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_BEFRIEND);


	LIGHT_DESC Desc = {};
	Desc.eType = LIGHT_TYPE::DIRECTIONAL;
	Desc.vDir = _float4(-1.6f, 1.28f, -1.f, 0.f);
	Desc.vDiffuse = _float4(0.55f, 0.62f, 0.80f, 1.0f);
	Desc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	Desc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);
	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

	/* 집 조명 */
	Desc.eType = LIGHT_TYPE::POINT;
	Desc.vPos = _float4(-6.8f, 27.7001f, 26.1f, 1.f);
	Desc.fRange = 60.f;
	Desc.vDiffuse = _float4(1.00f, 0.97f, 0.92f, 1.0f);
	Desc.vAmbient = _float4(0.33f, 0.31f, 0.28f, 1.0f);
	Desc.vSpecular = _float4(0.f, 0.f, 0.f, 1.0f);
	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

	return S_OK;

}

HRESULT CPlace_Event_BeFriend::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 16.8f, 10.6f, 7.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 0.f, 5.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);

	return S_OK;
}


CPlace_Event_BeFriend* CPlace_Event_BeFriend::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Event_BeFriend* pPlaceMain = new CPlace_Event_BeFriend(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Event_BeFriend::Free()
{
	CPlace::Free();
}
