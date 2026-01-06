#include "Place_Event_Love.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

/* Manager */
#include "MiiManager.h"
#include "ApartManager.h"
#include "GameDirector.h"

/* Actor */
#include "Park_Actor.h"
#include "SEMii.h"

CPlace_Event_Love::CPlace_Event_Love(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Event_Love::Initialize()
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

int32_t CPlace_Event_Love::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_Event_Love::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("ø¨¿Œ¿Ã µ≈."));
	return S_OK;
}

HRESULT CPlace_Event_Love::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("ø¨¿Œ¿Ã µ≈."));
	return S_OK;
}

HRESULT CPlace_Event_Love::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_LOVE);
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	// ProtoTag_Actor_Park
	{
		CPark_Actor::PARK_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		Desc.fCX = 1.f;
		Desc.fCY = 1.f;
		Desc.fCZ = 1.f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_Actor_Park"), &Desc);

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
				Desc.vPos = XMFLOAT4{ 50.f, 0.f, -3.f, 1.f };
				Desc.iIndex = CApartManager::GetInstance()->Get_SrcMiiIndex();
			}
			else
			{
				Desc.vPos = XMFLOAT4{ 50.f, 0.f, 10.f, 1.f };
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

HRESULT CPlace_Event_Love::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_LOVE);

	return S_OK;
}

HRESULT CPlace_Event_Love::Ready_Environment_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::EVENT_LOVE);

	return S_OK;
}

HRESULT CPlace_Event_Love::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 90.f, 10.6f, 0.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 50.f, 5.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);

	return S_OK;
}


CPlace_Event_Love* CPlace_Event_Love::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Event_Love* pPlaceMain = new CPlace_Event_Love(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Event_Love::Free()
{
	CPlace::Free();
}
