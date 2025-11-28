#include "MainApp.h"
#include "GameInstance.h"
#include "Place_Loading.h"

#include "MiiManager.h"

// Actor
#include "WorldCamera.h"
#include "UICamera.h"

// Componenet
#include "Mii.h"
#include "MiiBody.h"
#include "MiiHead.h"
#include "MiiHair.h"
#include "MiiEyes.h"
#include "MiiEyebrow.h"
#include "MiiNose.h"
#include "MiiMouth.h"

// UI
#include "LoadingBG.h"


USING(Client)

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	  AllocConsole();                           // 콘솔 창 생성
	  FILE* fp;								  
	  freopen_s(&fp, "CONOUT$", "w", stdout);   // 표준 출력(stdout)을 콘솔로 리디렉션
	  freopen_s(&fp, "CONOUT$", "w", stderr);   // 표준 에러(stderr)도 콘솔로 리디렉션
	  freopen_s(&fp, "CONIN$", "r", stdin);     // 표준 입력(stdin)도 콘솔로 리디렉션

	ENGINE_DESC EngineDesc = {};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.hInst = g_hInst;
	EngineDesc.WinMode = Engine::WINMODE::WIN;
	EngineDesc.iWinCX = WINCX;
	EngineDesc.iWinCY = WINCY;
	EngineDesc.PlaceCount = static_cast<uint32_t>(PLACEID::END);

	if(FAILED(m_pGameInstance->Initialize(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;
	
	if (FAILED(Ready_Static()))
		return E_FAIL;

	// 다 지우고 다음 씬으로 넘어가고 싶다.
	if (FAILED(m_pGameInstance->PlaceManager_Change_Place(
		static_cast<uint32_t>(PLACEID::LOADING),	// 로딩씬으로 갈래
		CPlace_Loading::Create(						
			m_pDevice,
			m_pContext,
			PLACEID::ISLAND))))		// 로딩씬을 지우고
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
			 tmp = CShaderCom::Create(m_pDevice,m_pContext,TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Tex.hlsl"),
				 Engine::VTX_TEX::Elements,Engine::VTX_TEX::iNumElements);
			 if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTexShader"), tmp)))
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
				static_cast<uint32_t>(PLACEID::STATIC),
				TEXT("ProtoTag_Component_VtxMeshShader"),
				tmp)))
				return E_FAIL;
		}

		// ProtoTag_Component_VtxAnimMeshShader
		{
			tmp = CShaderCom::Create(
				m_pDevice,
				m_pContext,
				TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Anim_Mesh.hlsl"),
				Engine::VTX_TEX_ANIM_MESH::Elements,
				Engine::VTX_TEX_ANIM_MESH::iNumElements);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACEID::STATIC),
				TEXT("ProtoTag_Component_VtxAnimMeshShader"),
				tmp)))
				return E_FAIL;
		}

	 	// ProtoTag_Component_RectBuffer
	 	{
	 		tmp = CRectBufferCom::Create(m_pDevice, m_pContext);
	 
	 		if (FAILED(m_pGameInstance->Proto_Add_Prototype(
	 			static_cast<uint32_t>(PLACEID::STATIC),
	 			TEXT("ProtoTag_Component_RectBuffer"),
	 			tmp)))
	 			return E_FAIL;
	 	}

		if (FAILED(Ready_ModelCom()))
			return E_FAIL;
		if (FAILED(Ready_HairModelCom()))
			return E_FAIL;
		if (FAILED(Ready_HeadModelCom()))
			return E_FAIL;
		if (FAILED(Ready_FacePartsTexture()))
			return E_FAIL;

		// ProtoTag_Component_CommonButtonTexture
		{
			tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/Sprite_CityHall_Icon/CMButton%d.png"), 2);
			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_CommonButtonTexture"), tmp)))
				return E_FAIL;
		}
	 }

	// Actor
	{
		// ProtoTag_Actor_WorldCamera
		{
			tmp = CWorldCamera::Create(m_pDevice, m_pContext);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACEID::STATIC),
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
				static_cast<uint32_t>(PLACEID::STATIC),
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
				static_cast<uint32_t>(PLACEID::STATIC),
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
				static_cast<uint32_t>(PLACEID::STATIC),
				TEXT("ProtoTag_Actor_UICamera"),
				&tUICameraDesc);

			if (nullptr == tmp)
				return E_FAIL;

			if (FAILED(m_pGameInstance->CameraManager_Add_Camera(
				TEXT("UI_Camera"),
				dynamic_cast<CCamera*>(tmp))))
				return E_FAIL;
		}


				// ProtoTag_Component_Island_Model
			// ProtoTag_Component_Island_seModel
		{
			XMMATRIX initMatrix = XMMatrixIdentity();
			initMatrix;// = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

			tmp = CSEModel::Create(
				m_pDevice,
				m_pContext,
				ANIM_TYPE::NONEANIM,
				"Island",
				initMatrix);

			if (FAILED(m_pGameInstance->Proto_Add_Prototype(
				static_cast<uint32_t>(PLACEID::STATIC),
				TEXT("ProtoTag_Component_Island_seModel"),
				tmp)))
				return E_FAIL;
		}

		Ready_ModelActor();
	}


	// UI
	{
		// ProtoTag_UI_LoadingBG
		{
			tmp = CLoadingBG::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_LoadingBG"), tmp)))
				return E_FAIL;
		}
	}

	

	return S_OK;
}

HRESULT CMainApp::Ready_ModelCom()
{	
	CObject* tmp = { nullptr };

	// ProtoTag_Component_AnimationTester
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;// = XMMatrixRotationY(XMConvertToRadians(180.f));

		tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_AnimationTester", ANIM_TYPE::ANIM, "../../../../../../Resource/dev/05_Anim_fbx/CM_Wait00.fbx", InitMatrix, "AnimTester");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_AnimationTester"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Component_BabyJumper
	// ProtoTag_Component_seBabyJumper
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;// = XMMatrixRotationY(XMConvertToRadians(180.f));

		tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_BabyJumper", ANIM_TYPE::ANIM, "../../Resource/Model_Body/BabyJumper/BabyJumper.fbx", InitMatrix, "BabyJumper");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_BabyJumper"), tmp)))
			return E_FAIL;

		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "BabyJumper", InitMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_seBabyJumper"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Component_ChineseDress
	// ProtoTag_Component_seChineseDress
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;// = XMMatrixRotationY(XMConvertToRadians(180.f));

		tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_ChineseDress", ANIM_TYPE::ANIM, "../../Resource/Model_Body/ChineseDress/ChineseDress.fbx", InitMatrix, "ChineseDress");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_ChineseDress"), tmp)))
			return E_FAIL;

		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "ChineseDress", InitMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_seChineseDress"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Component_CamouflageJacket
	// ProtoTag_Component_seCamouflageJacket
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix; //= XMMatrixRotationY(XMConvertToRadians(180.f));

		tmp = CModel::Create(m_pDevice, m_pContext, "ProtoTag_Component_CamouflageJacket", ANIM_TYPE::ANIM, "../../Resource/Model_Body/CamouflageJacket/CamouflageJacket.fbx", InitMatrix, "CamouflageJacket");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_CamouflageJacket"), tmp)))
			return E_FAIL;

		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, "CamouflageJacket", InitMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_seCamouflageJacket"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Ready_HeadModelCom()
{
	CObject* tmp = { nullptr };
	
	// ProtoTag_Component_Head1
	// ProtoTag_Component_seHead1
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;// = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));// *XMMatrixRotationZ(XMConvertToRadians(90.f));
		/* ProtoTag_Component_Head9 */

		for (uint32_t i = 1; i < 9; ++i)
		{
			_string Name = "Head";
			_wstring Prototag = L"ProtoTag_Component_seHead";
			Name += to_string(i);
			Prototag += to_wstring(i);

			// _string path = "../../Resource/Model_Head/Head";
			// _string path2 = ".fbx";
			// path += to_string(i);
			// path += path2;
			// _string Prototag1 = "ProtoTag_Component_Head";
			// _wstring wPrototag1 = L"ProtoTag_Component_Head";
			//
			// Prototag1 += to_string(i);
			// wPrototag1 += to_wstring(i);
			// 
			// tmp = CModel::Create(m_pDevice, m_pContext, Prototag1, ANIM_TYPE::NONEANIM, path.c_str(), InitMatrix, Name.c_str());
			// if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), wPrototag1, tmp)))
			// 	return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, Name.c_str(), InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), Prototag, tmp)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CMainApp::Ready_HairModelCom()
{
	CObject* tmp = { nullptr };

	/* ProtoTag_Component_Hair*/
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;
		for (uint32_t i = 1; i < 12; ++i)
		{
			_string seSaveName = "Hair";
			_wstring seProtoTag = L"ProtoTag_Component_seHair";
			seSaveName += to_string(i);
			seProtoTag += to_wstring(i);

			 _string path = "../../Resource/Model_Hair/Hair";
			 _string path2 = ".fbx";
			 path += to_string(i);
			 path += path2;
			 _string ProtoTag = "ProtoTag_Component_Hair";
			 _wstring wPrototag = L"ProtoTag_Component_Hair";
			
			 ProtoTag += to_string(i);
			 wPrototag += to_wstring(i);
			 
			 tmp = CModel::Create(m_pDevice, m_pContext, ProtoTag, ANIM_TYPE::NONEANIM, path.c_str(), InitMatrix, seSaveName.c_str());
			 if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), wPrototag, tmp)))
			 	return E_FAIL;

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, seSaveName.c_str(), InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), seProtoTag, tmp)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CMainApp::Ready_FacePartsTexture()
{
	CObject* tmp = { nullptr };

	// ProtoTag_Component_FacePartsTexture
	tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/MiiEditor/MiiParts/%d.png"), 118);
	if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_FacePartsTexture"), tmp)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_ModelActor()
{
	CObject* tmp = { nullptr };

	// ProtoTag_Actor_MiiBody
	{
		tmp = CMiiBody::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiBody"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiHead
	{
		tmp = CMiiHead::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiHead"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiHair
	{
		tmp = CMiiHair::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiHair"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiEyes
	{
		tmp = CMiiEyes::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiEyes"), tmp)))
			return E_FAIL;
	}	
	
	// ProtoTag_Actor_MiiEyebrow
	{
		tmp = CMiiEyebrow::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiEyebrow"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiNose
	{
		tmp = CMiiNose::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiNose"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiMouth
	{
		tmp = CMiiMouth::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiMouth"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_Mii
	{
		tmp = CMii::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_Mii"), tmp)))
			return E_FAIL;
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

	CMiiManager::DestroyInstance();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	m_pGameInstance->Clear_Engine();


	Safe_Release(m_pGameInstance);
}
