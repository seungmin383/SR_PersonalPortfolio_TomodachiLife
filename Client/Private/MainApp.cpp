#include "MainApp.h"
#include "GameInstance.h"
#include "Place_Loading.h"

#include "WorldCamera.h"
#include "UICamera.h"

USING(Client)

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	 //AllocConsole();                          // 콘솔 창 생성
	 //FILE* fp;
	 //freopen_s(&fp, "CONOUT$", "w", stdout);  // 표준 출력(stdout)을 콘솔로 리디렉션
	 //freopen_s(&fp, "CONOUT$", "w", stderr);  // 표준 에러(stderr)도 콘솔로 리디렉션
	 //freopen_s(&fp, "CONIN$", "r", stdin);    // 표준 입력(stdin)도 콘솔로 리디렉션

	ENGINE_DESC EngineDesc = {};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.hInst = g_hInst;
	EngineDesc.WinMode = Engine::WINMODE::WIN;
	EngineDesc.iWinCX = WINCX;
	EngineDesc.iWinCY = WINCY;
	EngineDesc.PlaceCount = static_cast<uint32_t>(PLACE_ID::END);

	if(FAILED(m_pGameInstance->Initialize(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;
	
	if (FAILED(Ready_Static()))
		return E_FAIL;

	// 다 지우고 다음 씬으로 넘어가고 싶다.
	if (FAILED(m_pGameInstance->PlaceManager_Change_Place(
		static_cast<uint32_t>(PLACE_ID::LOADING),	// 로딩씬으로 갈래
		CPlace_Loading::Create(						
			m_pDevice,
			m_pContext,
			PLACE_ID::ISLAND))))		// 로딩씬을 지우고
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

	 // Component
	 {
	 	// ProtoTag_Component_VtxTexShader
	 	{
	 		tmp = CShaderCom::Create(
	 			m_pDevice,
	 			m_pContext,
	 			TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Tex.hlsl"),
	 			Engine::VTX_TEX::Elements,
	 			Engine::VTX_TEX::iNumElements);
	 
	 		if (FAILED(m_pGameInstance->Proto_Add_Prototype(
	 			static_cast<uint32_t>(PLACE_ID::STATIC),
	 			TEXT("ProtoTag_Component_VtxTexShader"),
	 			tmp)))
	 			return E_FAIL;
	 	}

		// ProtoTag_Component_VtxMeshShader
		{
			tmp = CShaderCom::Create(
				m_pDevice,
				m_pContext,
				TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Mesh.hlsl"),
				Engine::VTX_TEX_MESH::Elements,
				Engine::VTX_TEX_MESH::iNumElements);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACE_ID::STATIC),
				TEXT("ProtoTag_Component_VtxMeshShader"),
				tmp)))
				return E_FAIL;
		}

	 	// ProtoTag_Component_RectBuffer
	 	{
	 		tmp = CRectBufferCom::Create(m_pDevice, m_pContext);
	 
	 		if (FAILED(m_pGameInstance->Proto_Add_Prototype(
	 			static_cast<uint32_t>(PLACE_ID::STATIC),
	 			TEXT("ProtoTag_Component_RectBuffer"),
	 			tmp)))
	 			return E_FAIL;
	 	}

	 }

	// Actor
	{
		// ProtoTag_Actor_WorldCamera
		{
			tmp = CWorldCamera::Create(m_pDevice, m_pContext);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACE_ID::STATIC),
				TEXT("ProtoTag_Actor_WorldCamera"),
				tmp)))
				return E_FAIL;

			CWorldCamera::WORLDCAMERA_DESC tWorldCameraDesc = {};
			tWorldCameraDesc.fSensor = 0.5f;
			tWorldCameraDesc.fAspect = 2.f;
			tWorldCameraDesc.fFov = XMConvertToRadians(45.f);
			tWorldCameraDesc.fFarZ = 1000.f;
			tWorldCameraDesc.fNearZ = 0.1f;
			tWorldCameraDesc.fWidth = static_cast<_float>(WINCX);
			tWorldCameraDesc.fHeight = static_cast<_float>(WINCY / 2);
			tWorldCameraDesc.fSpeedPerSecond = 5.f;
			tWorldCameraDesc.fRotationPerSecond = XMConvertToRadians(20.f);
			tWorldCameraDesc.vEye = _float3(2.f, 5.f, -30.f);
			tWorldCameraDesc.vAt = _float3(2.f, 0.f, 0.f);
			tWorldCameraDesc.fCX = 1.f;
			tWorldCameraDesc.fCY = 1.f;
			tWorldCameraDesc.fCZ = 1.f;
			tWorldCameraDesc.fX = 0.f;
			tWorldCameraDesc.fY = 0.f;
			tWorldCameraDesc.fZ = 0.f;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACE_ID::STATIC),
				TEXT("ProtoTag_Actor_WorldCamera"),
				&tWorldCameraDesc);

			if (nullptr == tmp)
				return E_FAIL;


			if (FAILED(m_pGameInstance->CameraManager_Add_Camera(
				TEXT("World_Camera"),
				dynamic_cast<CCamera*>(tmp))))
				return E_FAIL;
		}


		// ProtoTag_Actor_UICamera
		{
			tmp = CUICamera::Create(m_pDevice, m_pContext);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACE_ID::STATIC),
				TEXT("ProtoTag_Actor_UICamera"),
				tmp)))
				return E_FAIL;

			CUICamera::UICAMERA_DESC tUICameraDesc = {};
			tUICameraDesc.fSensor = 1.f;
			tUICameraDesc.fAspect = 2.f;
			tUICameraDesc.fFov = XMConvertToRadians(90.f);
			tUICameraDesc.fFarZ = 1.f;
			tUICameraDesc.fNearZ = 0.1f;
			tUICameraDesc.fWidth = static_cast<_float>(WINCX);
			tUICameraDesc.fHeight = static_cast<_float>(WINCY / 2);
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

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACE_ID::STATIC),
				TEXT("ProtoTag_Actor_UICamera"),
				&tUICameraDesc);

			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->CameraManager_Add_Camera(
				TEXT("UI_Camera"),
				dynamic_cast<CCamera*>(tmp))))
				return E_FAIL;
		}

	}


	// UI
	{

	}

	

	return S_OK;
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

	FreeConsole();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	m_pGameInstance->Clear_Engine();


	Safe_Release(m_pGameInstance);
}
