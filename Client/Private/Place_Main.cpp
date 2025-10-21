#include "Place_Main.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "UICamera.h"

CPlace_Main::CPlace_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Main::Initialize()
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
	//m_pWorldCamera->Update_PipeLine();
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

HRESULT CPlace_Main::Ready_Actor_Layer()
{
	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("ProtoTag_Actor_Island"),
		nullptr);

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

HRESULT CPlace_Main::Ready_UI_Layer()
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


HRESULT CPlace_Main::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Main::Ready_Camera_Layer()
{
	CUICamera::UICAMERA_DESC tDesc = {};
	tDesc.fSensor = 1.f;
	tDesc.fAspect = 2.f;
	tDesc.fFov = XMConvertToRadians(90.f);
	tDesc.fFarZ = 1.f;
	tDesc.fNearZ = 0.1f;
	tDesc.fHeight = static_cast<_float>(WINCX);
	tDesc.fWidth = static_cast<_float>(WINCY / 2);
	tDesc.fSpeedPerSecond = 0.f;
	tDesc.fRotationPerSecond = XMConvertToRadians(20.f);
	tDesc.vEye = _float3(0.f, 0.f, -10.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);

	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::STATIC),
		TEXT("ProtoTag_Actor_UICamera"),
		&tDesc);

	m_pUICamera = dynamic_cast<CCamera*>(tmp);

	if (nullptr == m_pUICamera)
		return E_FAIL;
	Safe_AddRef(m_pUICamera);

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("LayerTag_Camera"),
		tmp)))
	{
		Safe_Release(tmp);
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

	CPlace::Free();
}
