#include "Place_Main.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

CPlace_Main::CPlace_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Main::Initialize()
{
	if (FAILED(Ready_Main_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Main_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Main_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Main_Camera_Layer()))
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_Main::Update(_float fDT)
{
	if (GetAsyncKeyState('A') & 0x8000)
		m_pGameInstance->PlaceManager_Change_Place(
			static_cast<uint32_t>(PLACE_ID::SETTING),
			Engine::RETAIN_POLICY::CLEAR_ALL,
			CPlace_Loading::Create(
				m_pDevice,
				m_pContext,
				PLACE_ID::SETTING,
				RETAIN_POLICY::CLEAR_LOADING));

	return 0;
}

HRESULT CPlace_Main::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("메인레벨입니다."));
	static_cast<CWorldCamera*>(m_pWorldCamera)->Ready_Render();
	//m_pGameInstance->PipeLine_Calc_ProjMatrix(m_pWorldCamera);
	return S_OK;
}

HRESULT CPlace_Main::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("메인레벨입니다."));
	static_cast<CUICamera*>(m_pUICamera)->Ready_Render();
	//m_pGameInstance->PipeLine_Calc_ProjMatrix(m_pUICamera);
	return S_OK;
}

HRESULT CPlace_Main::Ready_Main_Actor_Layer()
{
	CActor::ACTOR_DESC tDesc = {};
	tDesc.fX = 0.f;
	tDesc.fY = 0.f;
	tDesc.fZ = 0.f;
	tDesc.fCX = 1.f;
	tDesc.fCY = 1.f;
	tDesc.fCZ = 1.f;
	tDesc.fSpeedPerSecond = 0.f;
	tDesc.fRotationPerSecond = XMConvertToRadians(20.f);

	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("ProtoTag_Actor_Island"),
		&tDesc);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("LayerTag_Actor"),
		tmp)))
	{
		Safe_Release(tmp);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Main::Ready_Main_UI_Layer()
{
	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("ProtoTag_UI_Food"),
		nullptr);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("LayerTag_UI"),
		tmp)))
	{
		Safe_Release(tmp);
		return E_FAIL;
	}


	return S_OK;
}


HRESULT CPlace_Main::Ready_Main_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Main::Ready_Main_Camera_Layer()
{
	CUICamera::UICAMERA_DESC tUICameraDesc = {};
	tUICameraDesc.fSensor = 1.f;
	tUICameraDesc.fAspect = 2.f;
	tUICameraDesc.fFov = XMConvertToRadians(90.f);
	tUICameraDesc.fFarZ = 1.f;
	tUICameraDesc.fNearZ = 0.1f;
	tUICameraDesc.fHeight = static_cast<_float>(WINCX);
	tUICameraDesc.fWidth = static_cast<_float>(WINCY / 2);
	tUICameraDesc.fSpeedPerSecond = 0.f;
	tUICameraDesc.fRotationPerSecond = XMConvertToRadians(20.f);
	tUICameraDesc.vEye = _float3(0.f, 0.f, 0.f);
	tUICameraDesc.vAt = _float3(0.f, 0.f, 1.f);
	tUICameraDesc.fCX = 1.f;
	tUICameraDesc.fCY = 1.f;
	tUICameraDesc.fCZ = 1.f;
	tUICameraDesc.fX = 0.f;
	tUICameraDesc.fY = 0.f;
	tUICameraDesc.fZ = 0.f;
	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::STATIC),
		TEXT("ProtoTag_Actor_UICamera"),
		&tUICameraDesc);

	if (nullptr == tmp)
		return E_FAIL;

	m_pUICamera = tmp;
	Safe_AddRef(m_pUICamera);

	if (FAILED(m_pGameInstance->CameraManager_Add_Camera(
		TEXT("UI_Camera"),
		dynamic_cast<CCamera*>(tmp))))
	{
		Safe_Release(m_pUICamera);
		return E_FAIL;
	}


	CWorldCamera::WORLDCAMERA_DESC tWorldCameraDesc = {};
	tWorldCameraDesc.fSensor = 1.f;
	tWorldCameraDesc.fAspect = 2.f;
	tWorldCameraDesc.fFov = XMConvertToRadians(90.f);
	tWorldCameraDesc.fFarZ = 1000.f;
	tWorldCameraDesc.fNearZ = 0.1f;
	tWorldCameraDesc.fHeight = static_cast<_float>(WINCX);
	tWorldCameraDesc.fWidth = static_cast<_float>(WINCY/2);
	tWorldCameraDesc.fSpeedPerSecond = 10.f;
	tWorldCameraDesc.fRotationPerSecond = XMConvertToRadians(20.f);
	tWorldCameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	tWorldCameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	tWorldCameraDesc.fCX = 1.f;
	tWorldCameraDesc.fCY = 1.f;
	tWorldCameraDesc.fCZ = 1.f;
	tWorldCameraDesc.fX = 0.f;
	tWorldCameraDesc.fY = 0.f;
	tWorldCameraDesc.fZ = 0.f;

	tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::STATIC),
		TEXT("ProtoTag_Actor_WorldCamera"),
		&tWorldCameraDesc);

	if (nullptr == tmp)
		return E_FAIL;

	m_pWorldCamera = tmp;
	Safe_AddRef(m_pWorldCamera);

	if (FAILED(m_pGameInstance->CameraManager_Add_Camera(
		TEXT("World_Camera"),
		dynamic_cast<CCamera*>(tmp))))
	{
		Safe_Release(m_pWorldCamera);
		return E_FAIL;
	}
	return S_OK;
}


CPlace_Main* CPlace_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Main* pPlaceMain = new CPlace_Main(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Main Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Main::Free()
{
	Safe_Release(m_pUICamera);
	Safe_Release(m_pWorldCamera);

	CPlace::Free();
}
