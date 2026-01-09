#include "MainApp.h"
#include "GameInstance.h"
#include "Place_Loading.h"


#include "MiiManager.h"
#include "ApartManager.h"
#include "ItemManager.h"
#include "NarrationManager.h"
#include "GameDirector.h"

// Actor
#include "WorldCamera.h"
#include "UICamera.h"
#include "Room.h"
#include "MiiEditorBG.h"
#include "LevelUp_Item.h"
#include "Food_Item.h"

// Componenet
#include "Mii.h"
#include "MiiBody.h"
#include "MiiHead.h"
#include "MiiHair.h"
#include "MiiEyes.h"
#include "MiiEyebrow.h"
#include "MiiNose.h"
#include "MiiMouth.h"

#include "SEMii.h"
#include "SEMiiBody.h"
#include "SEMiiHead.h"
#include "SEMiiHair.h"
#include "SEMiiEyes.h"
#include "SEMiiEyebrow.h"
#include "SEMiiNose.h"
#include "SEMiiMouth.h"

#include "Collider.h"

// UI
#include"YellowBackGround.h"
#include "Window_Button.h"
#include "Option_Button.h"
#include "Back_Button.h"
#include "BlueBackGround.h"
#include "BasicBackGround.h"
#include "FoodItem_Texture.h"
#include "PlayerInven_Button.h"
#include "Category_Button.h"
#include "IconTexture.h"
#include "Wallet.h"
#include "Narration_TextBox.h"
#include "NoneBack_TextBox.h"

USING(Client)

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);


	// ID3D11RasterizerState;
	// ID3D11BlendState;
	// ID3D11DepthStencilState;
	// 
	// D3D11_RASTERIZER_DESC;
	// 
	// D3D11_BLEND_DESC;
	// D3D11_RENDER_TARGET_BLEND_DESC;
	// 
	// D3D11_DEPTH_STENCIL_DESC;

	//m_pDevice->CreateRasterizerState();
	//m_pDevice->CreateBlendState();
	//m_pDevice->CreateDepthStencilState();

	//m_pContext->RSSetState();
	//m_pContext->OMSetBlendState();
	//m_pContext->OMSetDepthStencilState();
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
	if (FAILED(m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(PLACEID::LOADING),
		CPlace_Loading::Create(						
			m_pDevice,
			m_pContext,
			PLACEID::ISLAND))))	
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
	if (FAILED(Ready_Manager()))
		return E_FAIL;

	/* 컴포넌트 */
	if (FAILED(Ready_Static_Component()))
		return E_FAIL;

	if (FAILED(Ready_Static_Shader()))
		return E_FAIL;

	if (FAILED(Ready_Static_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Static_Buffer()))
		return E_FAIL;

	if (FAILED(Ready_Static_Model()))
		return E_FAIL;

	/* 액터 */
	if (FAILED(Ready_Static_Actor()))
		return E_FAIL;

	if (FAILED(Ready_Static_Camera()))
		return E_FAIL;

	if(FAILED(Ready_Mii()))
		return E_FAIL;

	/* 유아이 */
	if (FAILED(Ready_StaticUI()))
		return E_FAIL;

	/* 아이템 */
	if (FAILED(Ready_Static_Item()))
		return E_FAIL;

	/* 폰트 */
	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Static_Shader()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 버텍스 텍스 */
	// ProtoTag_Component_VtxTex_Shader
	{
		tmp = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Tex.hlsl"),
			Engine::VTX_TEX::Elements, Engine::VTX_TEX::iNumElements);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VtxTex_Shader"), tmp)))
			return E_FAIL;
	}

	/* 버텍스 메쉬 */
   // ProtoTag_Component_VtxMesh_Shader
	{
		tmp = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Mesh.hlsl"),
			Engine::VTX_TEX_MESH::Elements, Engine::VTX_TEX_MESH::iNumElements);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VtxMesh_Shader"), tmp)))
			return E_FAIL;
	}

	/* 버텍스 애님메쉬 */
	// ProtoTag_Component_VtxAnimMesh_Shader
	{
		tmp = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Anim_Mesh.hlsl"),
			Engine::VTX_TEX_ANIM_MESH::Elements, Engine::VTX_TEX_ANIM_MESH::iNumElements);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VtxAnimMesh_Shader"), tmp)))
			return E_FAIL;
	}

	/* 버텍스 포즈 텍스 파티클 */
	// ProtoTag_Component_VtxPosTexParticle_Shader
	{
		tmp = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Tex_Particle.hlsl"),
			Engine::VTX_POS_TEX_PARTICLE::Elements, Engine::VTX_POS_TEX_PARTICLE::iNumElements);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VtxPosTexParticle_Shader"), tmp)))
			return E_FAIL;
	}

	/* 버텍스 포즈 파티클 */
	// ProtoTag_Component_VtxPosParticle_Shader
	{
		tmp = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Shader_Vtx_Pos_Particle.hlsl"),
			Engine::VTX_POS_PARTICLE::Elements, Engine::VTX_POS_PARTICLE::iNumElements);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_VtxPosParticle_Shader"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Static_Buffer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 네모 버퍼 */
	// ProtoTag_Component_RectBuffer
	{
		tmp = CRectBufferCom::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_RectBuffer"), tmp)))
			return E_FAIL;
	}

	/* 네모 파티클 버퍼 */
	// ProtoTag_Component_RectParticle_Buffer
	{
		CParticleBufferCom::PARTICLE_DESC Desc = {};
		Desc.iNumInst = 10000;
		Desc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };
		Desc.vRange = XMFLOAT3{ 100.f, 1.f, 100.f };
		Desc.vSize = XMFLOAT2{ 0.1f, 0.3f };
		Desc.vSpeed = XMFLOAT2{ 2.f, 5.f };
		Desc.vLifeTime = XMFLOAT2{ 3.f,5.f };
		Desc.isLoop = TRUE;

		tmp = CParticleBufferCom_Rect::Create(m_pDevice, m_pContext, &Desc);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_RectParticle_Buffer"), tmp)))
			return E_FAIL;
	}
	
	/* 포인트 파티클 버퍼 */
	// ProtoTag_Component_PointParticle_Buffer
	{
		CParticleBufferCom::PARTICLE_DESC Desc = {};
		Desc.iNumInst = 800;
		Desc.vCenter = XMFLOAT3{ 0.f, 0.f, 0.f };
		Desc.vRange = XMFLOAT3{ 0.5f, 0.5f, 0.5f };
		Desc.vSize = XMFLOAT2{ 0.05f, 0.15f };
		Desc.vSpeed = XMFLOAT2{ 2.f, 5.f };
		Desc.vLifeTime = XMFLOAT2{ 1.f,1.5f };
		Desc.vPivot = XMFLOAT3{ 0.f, -0.5f, 0.f };
		Desc.isLoop = TRUE;

		tmp = CParticleBufferCom_Point::Create(m_pDevice, m_pContext, &Desc);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_PointParticle_Buffer"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Ready_Static_Model()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 섬 모델 */
	// ProtoTag_Component_Island_seModel
	{
		XMMATRIX initMatrix = XMMatrixIdentity();
		initMatrix;// = XMMatrixScaling(0.05f, 0.05f, 0.05f);// *XMMatrixRotationY(XMConvertToRadians(180.0f));

		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "Island", initMatrix);

		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Island_seModel"), tmp)))
			return E_FAIL;
	}

#ifdef _DEBUG
	///* 애니메이션 테스터 모델 */
	//// ProtoTag_Component_AnimationTester
	//{
	//	XMMATRIX InitMatrix = XMMatrixIdentity();
	//	InitMatrix;// = XMMatrixRotationY(XMConvertToRadians(180.f));

	//	/* 폴더 순회 / entry는 그 폴더 안의 파일이나 폴더 */
	//	for (const auto& entry : filesystem::directory_iterator("../../../../../../Resource/dev/05_Anim_fbx/"))
	//	{
	//		/* entry가 일반 파일인지 확인 / 폴더, 바로가기 등이면 false */
	//		if (!entry.is_regular_file())
	//			continue;

	//		/* 파일 경로에서 확장자만 추출 */
	//		_wstring strEXT = entry.path().extension().wstring();
	//		if (strEXT != L".fbx")
	//			continue;

	//		string FileStemA = entry.path().stem().string();
	//		wstring ProtoTagW = L"ProtoTag_Component_AnimationTester_" + entry.path().stem().wstring();

	//		tmp = CModel::Create(m_pDevice, m_pContext,
	//			ANIM_TYPE::ANIM,
	//			entry.path().generic_string().c_str(),
	//			InitMatrix,
	//			FileStemA.c_str());


	//		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, ProtoTagW, tmp)))
	//			return E_FAIL;
	//	}
	//}
#endif

	_wstring strClothModelTagArr[] = {
		TEXT("BabyJumper"),
		TEXT("CamouflageJacket"),
		TEXT("ChineseDress"),
		TEXT("FauxFarCoat"),
		TEXT("NintendoUniform"),
		TEXT("SilkBlouse"),
		TEXT("SoccerUniform"),
		TEXT("Toga"),
		TEXT("WaitressUniform"),
	};

	for (const _wstring& strTag : strClothModelTagArr)
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;// = XMMatrixRotationY(XMConvertToRadians(180.f));

#ifdef _DEBUG
		_wstring strFbxPath = L"../../Resource/Model_Body/" + strTag + L"/" + strTag + L".fbx";
		tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, WstrToStr(strFbxPath).c_str(), InitMatrix, WstrToStr(strTag).c_str());
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, (L"ProtoTag_Component_" + strTag).c_str(), tmp)))
			return E_FAIL;
#endif

		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, WstrToStr(strTag).c_str(), InitMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, (L"ProtoTag_Component_se" + strTag).c_str(), tmp)))
			return E_FAIL;
	}

	/* 주민 머리 모델 */
	// ProtoTag_Component_seHead1
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;// = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));// *XMMatrixRotationZ(XMConvertToRadians(90.f));

		for (uint32_t i = 1; i < 9; ++i)
		{
			_string SaveName = "Head" + to_string(i);
			_wstring sePrototag = L"ProtoTag_Component_seHead" + to_wstring(i);

#ifdef _DEBUG
			_string ProtoTag = "ProtoTag_Component_Head" + to_string(i);
			_string FilePath = "../../Resource/Model_Head/Head" + to_string(i) + ".fbx";

			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, FilePath.c_str(), InitMatrix, SaveName.c_str());
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, StrToWstr(ProtoTag), tmp)))
				return E_FAIL;
#endif

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, SaveName.c_str(), InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, sePrototag, tmp)))
				return E_FAIL;
		}
	}

	/* 주민 머리카락 모델 */
	// ProtoTag_Component_Hair
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
		InitMatrix;

		for (uint32_t i = 1; i < 12; ++i)
		{
			_string SaveName = "Hair" + to_string(i);
			_wstring seProtoTag = L"ProtoTag_Component_seHair" + to_wstring(i);

#ifdef _DEBUG
			_string ProtoTag = "ProtoTag_Component_Hair" + to_string(i);
			_string FilePath = "../../Resource/Model_Hair/Hair" + to_string(i) + ".fbx";

			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, FilePath.c_str(), InitMatrix, SaveName.c_str());
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, StrToWstr(ProtoTag), tmp)))
				return E_FAIL;
#endif

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, SaveName.c_str(), InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, seProtoTag, tmp)))
				return E_FAIL;
		}
	}

	/* 방 모델 */
	// ProtoTag_Component_Room
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			XMMATRIX InitMatrix = XMMatrixIdentity();

			_string strProtoTag		= "ProtoTag_Component_Room" + to_string(i);
			_wstring wstrProtoTag	= L"ProtoTag_Component_Room" + to_wstring(i);
			_wstring wstrSeProtoTag = L"ProtoTag_Component_seRoom" + to_wstring(i);
			_string strFilePath		= "../../Resource/Actor/room" + to_string(i) + "/room.fbx";
			_string strSaveName		= "room" + to_string(i);

#ifdef _DEBUG
			tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, strFilePath.c_str(), InitMatrix, strSaveName.c_str());
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, wstrProtoTag, tmp)))
				return E_FAIL;
#endif 

			tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::ANIM, strSaveName.c_str(), InitMatrix);
			if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, wstrSeProtoTag, tmp)))
				return E_FAIL;
		}
	}

	/* 배경 모델 */
	// ProtoTag_Componenet_MiiEditorBG
	{
		XMMATRIX InitMatrix = XMMatrixIdentity();
#ifdef _DEBUG
		tmp = CModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM,
			"../../Resource/00Model/BackGround - MiiEditor/EditorBG.fbx", InitMatrix, "EditorBG");
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Componenet_MiiEditorBG"), tmp)))
			return E_FAIL;

#endif 

		tmp = CSEModel::Create(m_pDevice, m_pContext, ANIM_TYPE::NONEANIM, "EditorBG", InitMatrix);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Componenet_seMiiEditorBG"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Ready_Static_Actor()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);
	
	/* 주민 방 */
	// ProtoTag_Actor_Room
	{
		tmp = CRoom::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Room"), tmp)))
			return E_FAIL;
	}

	/* 에디터 배경 */
	// ProtoTag_Actor_EditorBG
	{
		tmp = CMiiEditorBG::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_EditorBG"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Ready_Static_Camera()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 월드 카메라 */
	// ProtoTag_Actor_WorldCamera
	{
		tmp = CWorldCamera::Create(m_pDevice, m_pContext);

		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_WorldCamera"), tmp)))
			return E_FAIL;

		CWorldCamera::WORLDCAMERA_DESC tDesc = {};
		tDesc.fRotationPerSecond = XMConvertToRadians(60.f);
		tDesc.fHeight = static_cast<_float>(WINCY / 2);
		tDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		tDesc.fWidth = static_cast<_float>(WINCX);
		tDesc.fFov = XMConvertToRadians(45.f);
		tDesc.vEye = _float3(2.f, 5.f, -30.f);
		tDesc.vAt = _float3(2.f, 0.f, 0.f);
		tDesc.fSpeedPerSecond = 30.f;
		tDesc.fFarZ = 1000.f;
		tDesc.fSensor = 0.5f;
		tDesc.fAspect = 2.f;
		tDesc.fNearZ = 0.1f;
		tDesc.fCX = 1.f;
		tDesc.fCY = 1.f;
		tDesc.fCZ = 1.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_Actor_WorldCamera"), &tDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->CameraManager_Add_Camera(TEXT("World_Camera"),	dynamic_cast<CCamera*>(tmp))))
			return E_FAIL;
	}


	// ProtoTag_Actor_UICamera
	{
		tmp = CUICamera::Create(m_pDevice, m_pContext);

		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_UICamera"), tmp)))
			return E_FAIL;

		CUICamera::UICAMERA_DESC tDesc = {};
		tDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tDesc.fHeight = static_cast<_float>(WINCY / 2);
		tDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
		tDesc.fWidth = static_cast<_float>(WINCX);
		tDesc.fFov = XMConvertToRadians(90.f);
		tDesc.vEye = _float3(0.f, 0.f, 0.f);
		tDesc.vAt = _float3(0.f, 0.f, 1.f);
		tDesc.fSpeedPerSecond = 0.f;
		tDesc.fNearZ = 0.1f;
		tDesc.fSensor = 1.f;
		tDesc.fAspect = 2.f;
		tDesc.fFarZ = 1.f;
		tDesc.fCX = 1.f;
		tDesc.fCY = 1.f;
		tDesc.fCZ = 1.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, TEXT("ProtoTag_Actor_UICamera"), &tDesc);

		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->CameraManager_Add_Camera(TEXT("UI_Camera"), dynamic_cast<CCamera*>(tmp))))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CMainApp::Ready_Manager()
{
	if (FAILED(CApartManager::GetInstance()->Load_ApartData()))
		return E_FAIL;


	if (FAILED(CMiiManager::GetInstance()->Initialize()))
		return E_FAIL;	

	if (FAILED(CItemManager::GetInstance()->Initialize()))
		return E_FAIL;


	if (FAILED(CNarrationManager::GetInstance()->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Mii()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	// ProtoTag_Actor_MiiBody
	{
		tmp = CMiiBody::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiBody"), tmp)))
			return E_FAIL;

		tmp = CSEMiiBody::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiBody"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiHead
	{
		tmp = CMiiHead::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiHead"), tmp)))
			return E_FAIL;

		tmp = CSEMiiHead::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiHead"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiHair
	{
		tmp = CMiiHair::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiHair"), tmp)))
			return E_FAIL;

		tmp = CSEMiiHair::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiHair"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiEyes
	{
		tmp = CMiiEyes::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiEyes"), tmp)))
			return E_FAIL;

		tmp = CSEMiiEyes::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiEyes"), tmp)))
			return E_FAIL;
	}	
	
	// ProtoTag_Actor_MiiEyebrow
	{
		tmp = CMiiEyebrow::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiEyebrow"), tmp)))
			return E_FAIL;

		tmp = CSEMiiEyebrow::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiEyebrow"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiNose
	{
		tmp = CMiiNose::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiNose"), tmp)))
			return E_FAIL;

		tmp = CSEMiiNose::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiNose"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_MiiMouth
	{
		tmp = CMiiMouth::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_MiiMouth"), tmp)))
			return E_FAIL;

		tmp = CSEMiiMouth::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMiiMouth"), tmp)))
			return E_FAIL;
	}

	/* 레벨업 선물 아이템 */
	// ProtoTag_Actor_LevelUp_Item
	{
		tmp = CLevelUp_Item::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_LevelUp_Item"), tmp)))
			return E_FAIL;
	}

	/* 음식 아이템 */
	// ProtoTag_Actor_Food_Item
	{
		tmp = CFood_Item::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Food_Item"), tmp)))
			return E_FAIL;
	}

	// ProtoTag_Actor_Mii
	{
		tmp = CMii::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_Mii"), tmp)))
			return E_FAIL;

		tmp = CSEMii::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Actor_seMii"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Static_Component()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 구 콜라이더 */
	// ProtoTag_Component_SphereCollider
	{
		tmp = CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_SphereCollider"), tmp)))
			return E_FAIL;
	}

	/* 뇌 */
	// ProtoTag_Component_Brain
	{
		tmp = CBrain::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Brain"), tmp)))
			return E_FAIL;
	}

	/* 애니메이션 플레이어 */
	// ProtoTag_Component_AnimationPlayer
	{
		tmp = CAnimationPlayer::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_AnimationPlayer"), tmp)))
			return E_FAIL;	
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Static_Texture()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 창문 텍스쳐 */
	// ProtoTag_Component_WindowButton_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Window_Button/%d.png"), 7);
		if (nullptr == tmp)
			return E_FAIL;
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_WindowButton_Texture"), tmp)))
			return E_FAIL;
	}

	/* 옵션 버튼 텍스쳐 */
	// ProtoTag_Component_Option_Button_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Option_Button/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Option_Button_Texture"), tmp)))
			return E_FAIL;
	}

	/* 얼굴 파츠 텍스쳐 */
	// ProtoTag_Component_FaceParts_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/MiiEditor/MiiParts/%d.png"), 118);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_FaceParts_Texture"), tmp)))
			return E_FAIL;
	}

	/* 땡땡이 배경 텍스쳐 */
	// ProtoTag_Component_BackGround_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/BackGround_Texture/%d.dds"), 4);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_ColorDot_BackGround_Texture"), tmp)))
			return E_FAIL;
	}

	/* 뒤로가기 버튼 텍스쳐 */
	// ProtoTag_Component_Back_Button_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Back_Button/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Back_Button_Texture"), tmp)))
			return E_FAIL;
	}

	/* 레벨업 선물 텍스쳐 */
	// ProtoTag_Component_LevelUp_Item_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/LevelUP_Item_texture/%d.dds"), 28);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_LevelUp_Item_Texture"), tmp)))
			return E_FAIL;
	}

	/* 플레이어 인벤토리 텍스쳐 */
	// ProtoTag_Component_PlayerInventory_Button_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/PlayerInventory_Button/%d.dds"), 2);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_PlayerInventory_Button_Texture"), tmp)))
			return E_FAIL;
	}


	/* 카테고리 버튼 텍스쳐 */
	// ProtoTag_Component_Category_Button_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Category_Button/%d.dds"), 2);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Category_Button_Texture"), tmp)))
			return E_FAIL;
	}


	/* 아이콘 텍스쳐 */
	// ProtoTag_Component_IconTexture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Icon_Texture/%d.png"), 23);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_IconTexture"), tmp)))
			return E_FAIL;
	}

	/* 지갑 텍스쳐 */
	// ProtoTag_Component_PlayerMoney_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Wallet_Texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Wallet_Texture"), tmp)))
			return E_FAIL;
	}

	/* 나레이션 텍스트 박스 텍스쳐 */
	// ProtoTag_Component_Narration_TextBox_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Narration_TextBox_Texture/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Narration_TextBox_Texture"), tmp)))
			return E_FAIL;
	}

	/* 임시 텍스쳐들 */
	// ProtoTag_Component_Temp_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/Temp_Textures/%d.dds"), 1);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_Temp_Texture"), tmp)))
			return E_FAIL;
	}

	/* 시청 버튼 텍스쳐 */
	// ProtoTag_Component_CityhallMenu_Button_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/CityhallMenu_Button/%d.dds"), 2);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_CityhallMenu_Button_Texture"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_StaticUI()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 창문 버튼 */
	// ProtoTag_UI_WindowButton
	{
		tmp = CWindow_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_WindowButton"), tmp)))
			return E_FAIL;

	}

	/* 주황색 버튼 */
	// ProtoTag_UI_Option_Button
	{
		tmp = COption_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Option_Button"), tmp)))
			return E_FAIL;
	}

	/* 뒤로가기 버튼 */
	// ProtoTag_UI_Back_Button
	{
		tmp = CBack_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Back_Button"), tmp)))
			return E_FAIL;
	}
	
	/* 노랑 땡땡이 배경 */
	// ProtoTag_UI_YellowBackGround_Dot
	{
		tmp = CYellowBackGround::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_YellowBackGround_Dot"), tmp)))
			return E_FAIL;
	}

	/* 파란 떙땡이 배경 */
	// ProtoTag_UI_BlueBackGround_Dot
	{
		tmp = CBlueBackGround::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_BlueBackGround_Dot"), tmp)))
			return E_FAIL;
	}

	/* 노란 플레인 배경 */
	// ProtoTag_UI_BlueBackGround_Dot
	{
		tmp = CBasicBackGround::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_YellowBackGround_Plane"), tmp)))
			return E_FAIL;
	}

	/* 플레이어 인벤토리 버튼 */
	// ProtoTag_UI_PlayerInven_Button
	{
		tmp = CPlayerInven_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_PlayerInven_Button"), tmp)))
			return E_FAIL;
	}


	/* 카테고리 버튼 */
	// ProtoTag_UI_Category_Button
	{
		tmp = CCategory_Button::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Category_Button"), tmp)))
			return E_FAIL;
	}

	/* 아이콘 */
	// ProtoTag_UI_Icon
	{
		tmp = CIconTexture::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Icon"), tmp)))
			return E_FAIL;
	}


	/* 지갑 */
	// ProtoTag_UI_Wallet
	{
		tmp = CWallet::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Wallet"), tmp)))
			return E_FAIL;
	}

	/* 나레이션 텍스트 박스 */
	// ProtoTag_UI_Narration_TextBox
	{
		tmp = CNarration_TextBox::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_Narration_TextBox"), tmp)))
			return E_FAIL;
	}

	/* 뒷배경이 없는 텍스트 박스 */
	// ProtoTag_UI_NoneBack_TextBox
	{
		tmp = CNoneBack_TextBox::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_NoneBack_TextBox"), tmp)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Ready_Static_Item()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::STATIC);

	/* 음식 아이템 텍스쳐 */
	// ProtoTag_Component_FoodItem_Texture
	{
		tmp = CTextureCom::Create(m_pDevice, m_pContext, TEXT("../../Resource/UI/FoodItem_Texture/%d.dds"), 29);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_Component_FoodItem_Texture"), tmp)))
			return E_FAIL;
	}

	/* 음식 아이템*/
	// ProtoTag_UI_FoodItem
	{
		tmp = CFoodItem_Texture::Create(m_pDevice, m_pContext);
		if (FAILED(m_pGameInstance->Proto_Add_Prototype(iPlaceID, TEXT("ProtoTag_UI_FoodItem"), tmp)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	if (FAILED(m_pGameInstance->FontManager_Add_Font(TEXT("Basic_Font_18"), TEXT("../../Resource/Fonts/Basic_Font_18.spritefont"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->FontManager_Add_Font(TEXT("Basic_Font_12"), TEXT("../../Resource/Fonts/Basic_Font_12.spritefont"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->FontManager_Add_Font(TEXT("Basic_Font_14"), TEXT("../../Resource/Fonts/Basic_Font_14.spritefont"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->FontManager_Add_Font(TEXT("Basic_Font_16"), TEXT("../../Resource/Fonts/Basic_Font_16.spritefont"))))
		return E_FAIL;	

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
	CApartManager::DestroyInstance();
	CItemManager::DestroyInstance();
	CNarrationManager::DestroyInstance();
	CGameDirector::DestroyInstance();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	m_pGameInstance->Clear_Engine();


	Safe_Release(m_pGameInstance);
}
