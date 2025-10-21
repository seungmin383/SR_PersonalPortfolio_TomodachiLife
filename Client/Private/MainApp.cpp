#include "MainApp.h"
#include "GameInstance.h"
#include "Place_Loading.h"

#include "MainCameraTop.h"
#include "UICamera.h"

USING(Client)

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC EngineDesc = {};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.WinMode = Engine::WINMODE::WIN;
	EngineDesc.iWinCX = WINCX;
	EngineDesc.iWinCY = WINCY;
	EngineDesc.PlaceCount = static_cast<uint32_t>(PLACE_ID::END);

	if(FAILED(m_pGameInstance->Initialize(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	// // 만약 내가 현재 씬을 유지하고, 다음 씬으로 넘어가고 싶다.
	// if (FAILED(m_pGameInstance->PlaceManager_Change_Place(
	// 	static_cast<uint32_t>(PLACE_ID::LOADING),	// 로딩씬으로 갈래
	// 	RETAIN_POLICY::RETAIN,						// 이전껀 유지하고
	// 	CPlace_Loading::Create(						// 로딩씬 포인터
	// 		m_pDevice,
	// 		m_pContext,
	// 		PLACE_ID::MAIN,							// 로딩 끝나면 메인으로 갈래
	// 		RETAIN_POLICY::CLEAR_LOADING))))		// 로딩씬을 지우고
	// 	return E_FAIL;	
	
	if (FAILED(Ready_Static()))
		return E_FAIL;

	// 다 지우고 다음 씬으로 넘어가고 싶다.
	if (FAILED(m_pGameInstance->PlaceManager_Change_Place(
		static_cast<uint32_t>(PLACE_ID::LOADING),	// 로딩씬으로 갈래
		RETAIN_POLICY::CLEAR_ALL,					// 다 지우고
		CPlace_Loading::Create(						
			m_pDevice,
			m_pContext,
			PLACE_ID::MAIN,							// 로딩 끝나면 메인으로 갈래
			RETAIN_POLICY::CLEAR_LOADING))))		// 로딩씬을 지우고
		return E_FAIL;


	return S_OK;
}

int32_t CMainApp::Update_Priority(_float _fDT)
{
	return m_pGameInstance->Update_Priority(_fDT);
}

int32_t CMainApp::Update(_float _fDT)
{
	return m_pGameInstance->Update(_fDT);
}

void CMainApp::Update_Late(_float _fDT)
{
	m_pGameInstance->Update_Late(_fDT);
}

void CMainApp::Render()
{
	m_pGameInstance->Begin_Render();

	m_pGameInstance->GraphicDevice_Bind_Viewport(VIEWPORT_TYPE::TOP);
	m_pGameInstance->Render_Top();

	m_pGameInstance->GraphicDevice_Bind_Viewport(VIEWPORT_TYPE::BOTTOM);
	m_pGameInstance->Render_Bottom();

	m_pGameInstance->End_Render();
}

HRESULT CMainApp::Ready_Static()
{
	CObject* tmp = { nullptr };

	// Main Camera Top //
	tmp = CMainCameraTop::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(Client::PLACE_ID::STATIC),
		TEXT("ProtoTag_Actor_MainCameraTop"),
		tmp)))
		return E_FAIL;


	// Main Camera Bottom //
	tmp = CUICamera::Create(m_pDevice, m_pContext);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Proto_Add_Prototype(
		static_cast<uint32_t>(Client::PLACE_ID::STATIC),
		TEXT("ProtoTag_Actor_UICamera"),
		tmp)))
		return E_FAIL;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (nullptr == pMainApp)
	{
		MSG_BOX("ErrCode : MainApp Create Failed");
		Safe_Release(pMainApp);
		return nullptr;
	}

	if (FAILED(pMainApp->Initialize()))
	{
		MSG_BOX("ErrCode : MainApp Initialize Failed");
		Safe_Release(pMainApp);
		return nullptr;
	}

	return pMainApp;
}

void CMainApp::Free()
{
	CBase::Free();


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Clear_Engine();

	Safe_Release(m_pGameInstance);
}
