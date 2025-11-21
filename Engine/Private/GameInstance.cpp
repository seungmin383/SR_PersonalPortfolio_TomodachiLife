#include "GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Place_Manager.h"
#include "Render_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Camera.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "MouseManager.h"
#include "FileManager.h"
#include "ButtonManager.h"
#include "Button.h"
#include "AnimationManager.h"

#define CONFIG_FILE_PATH "../../SharedProjects/GAME/GameData/config.json"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize(
	const ENGINE_DESC& Engine_desc,
	ID3D11Device** ppDevice,
	ID3D11DeviceContext** ppContext)
{
	//그래픽 디바이스
	m_pGraphicDevice = CGraphic_Device::Create(
		Engine_desc.hWnd,
		Engine_desc.WinMode,
		Engine_desc.iWinCX,
		Engine_desc.iWinCY,
		ppDevice,
		ppContext);

	if (nullptr == m_pGraphicDevice)
		return E_FAIL;


	//TimerManager
	{
		m_pTimerManager = CTimerManager::Create();
		if (nullptr == m_pTimerManager)
		{
			MSG_BOX("ErrCode : TimerManager Create Failed");
			return E_FAIL;
		}
	}

	//PlaceManager
	{
		m_pPlaceManager = CPlaceManager::Create();
		if (nullptr == m_pPlaceManager)
		{
			MSG_BOX("ErrCode : CPlaceManager Create Failed");
			return E_FAIL;
		}
	}

	//CObjectManager
	{
		m_pObjectManager = CObjectManager::Create(Engine_desc.PlaceCount);
		if (nullptr == m_pObjectManager) {
			MSG_BOX("ErrCode : CObjectManager Create Failed");
			return E_FAIL;
		}
	}

	//Prototype
	{
		m_pPrototype = CPrototype::Create(
			*ppDevice, 
			*ppContext, 
			Engine_desc.PlaceCount);

		if (nullptr == m_pPrototype) {
			MSG_BOX("ErrCode : CPrototype Create Failed");
			return E_FAIL;
		}
	}

	//RenderManager
	{
		m_pRenderManager = CRenderManager::Create(*ppDevice, *ppContext);
		if (nullptr == m_pRenderManager) {
			MSG_BOX("ErrCode : CRenderManager Create Failed");
			return E_FAIL;
		}
	}

	// PipeLine
	{
		m_pPipeLine = CPipeLine::Create();
		if (nullptr == m_pPipeLine) {
			MSG_BOX("ErrCode : CPipeLine Create Failed");
			return E_FAIL;
		}
	}

	// CameraManager
	{
		m_pCameraManager = CCameraManager::Create();
		if (nullptr == m_pCameraManager) {
			MSG_BOX("ErrCode : CCameraManager Create Failed");
			return E_FAIL;
		}
	}
	// InputDeice
	{
		m_pInput_Device = CInput_Device::Create(Engine_desc.hInst, Engine_desc.hWnd);
		if (nullptr == m_pInput_Device) {
			MSG_BOX("ErrCode : CInput_Device Create Failed");
			return E_FAIL;
		}
	}

	// MouseManager
	{
		m_pMouseManager = CMouseManager::Create(Engine_desc.hWnd, (Engine_desc.iWinCY / 2));
		if (nullptr == m_pInput_Device) {
			MSG_BOX("ErrCode : MouseManager Create Failed");
			return E_FAIL;
		}
	}

	// FileManager
	{
		m_pFileManager = CFileManager::Create(CONFIG_FILE_PATH);
		if (nullptr == m_pFileManager) {
			MSG_BOX("ErrCode : FileManager Create Failed");
			return E_FAIL;
		}
	}

	// ButtonManager
	{
		m_pButtonManager = CButtonManager::Create(
			*ppDevice, 
			*ppContext,
			Engine_desc.PlaceCount);
		if (nullptr == m_pFileManager) {
			MSG_BOX("ErrCode : ButtonManager Create Failed");
			return E_FAIL;
		}
	}

	// AnimationManager
	{
		m_pAnimationManager = CAnimationManager::Create();
		if(nullptr == m_pAnimationManager) {
			MSG_BOX("ErrCode : AnimationManager Create Failed");
			return E_FAIL;
		}
	}

	return S_OK;
}

int32_t CGameInstance::Update_Priority(_float fDT)
{

	m_pInput_Device->Update();
	m_pMouseManager->Update(fDT);
	m_pObjectManager->Update_Priority(fDT);
	m_pCameraManager->Update_Priority(fDT);
	return 0;
}

int32_t CGameInstance::Update(_float fDT)
{
	//게임 씬 매니저 업데이트			//주로 디버그용
	m_pPlaceManager->Update(fDT);
	//게임 오브젝트 매니저 업데이트		//진짜 업데이트
	
	m_pObjectManager->Update(fDT);
	m_pCameraManager->Update(fDT);
	return 	0;
}

void CGameInstance::Update_Late( _float fDT)
{
	m_pObjectManager->Update_Late(fDT);	
	m_pCameraManager->Update_Late(fDT);
}

void CGameInstance::Begin_Render()
{
	_float4		vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

	if (FAILED(GraphicDevice_Clear_BackBuffer_View(&vClearColor)))
		return;

	GraphicDevice_Clear_DepthStencil_View();
}

HRESULT CGameInstance::Render_Top()
{
	m_pPlaceManager->Render_Priority();

	//렌더 매니저 렌더
	if (FAILED(m_pRenderManager->Render_Top()))
	{
		MSG_BOX("ErrCode : RenderManager Render Top Failed");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CGameInstance::Render_Bottom()
{
	m_pPlaceManager->Render_Late();

	//렌더 매니저 렌더
	if (FAILED(m_pRenderManager->Render_Bottom()))
	{
		MSG_BOX("ErrCode : RenderManager Render Bottom Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGameInstance::End_Render()
{
	GraphicDevice_Present();
}

HRESULT CGameInstance::Clear_Resources(uint32_t PlaceID)
{
	if (FAILED(m_pObjectManager->Clear_Objects(PlaceID)))
		return E_FAIL;
	if (FAILED(m_pPrototype->Clear_Prototype(PlaceID)))
		return E_FAIL;

	return S_OK;
}

_float CGameInstance::Random(_float fMin, _float fMax)
{
	_float		fRandNormal = static_cast<_float>(rand()) / RAND_MAX;

	return (fMax - fMin) * fRandNormal + fMin;
}

#pragma region TIMER_MANAGER

inline _float CGameInstance::TimerManager_Get_DeltaTime(const _tchar* pTimerTag)
{
	return m_pTimerManager->Get_DeltaTime(pTimerTag);
}

inline HRESULT CGameInstance::TimerManager_Ready_Timer(const _tchar* pTimerTag)
{
	return m_pTimerManager->Ready_Timer(pTimerTag);
}

inline void CGameInstance::TimerManager_Calc_DeltaTime(const _tchar* pTimerTag)
{
	m_pTimerManager->Calc_DeltaTime(pTimerTag);
}

#pragma endregion

#pragma region GRAPHIC_DEVICE

inline HRESULT CGameInstance::GraphicDevice_Clear_BackBuffer_View(const _float4* pClearColor)
{
	return m_pGraphicDevice->Clear_BackBuffer_View(pClearColor);
}

inline HRESULT CGameInstance::GraphicDevice_Clear_DepthStencil_View()
{
	return m_pGraphicDevice->Clear_DepthStencil_View();
}

inline HRESULT CGameInstance::GraphicDevice_Present()
{
	return m_pGraphicDevice->Present();
}

void CGameInstance::GraphicDevice_Bind_Viewport(VIEWPORT_TYPE ViewportType)
{
	m_pGraphicDevice->Bind_Viewport(ViewportType);
}

void CGameInstance::GraphicDevice_Bind_RenderState(RENDER_STATE eRenderState)
{
	m_pGraphicDevice->Bind_RenderState(eRenderState);
}

#pragma endregion

#pragma region OBJECT_MANAGER

inline HRESULT CGameInstance::ObjectManager_Add_Object_in_Layer(
	uint32_t PlaceID,
	const _wstring LayerTag,
	CObject* pObject)
{
	return m_pObjectManager->Add_Object_in_Layer(PlaceID, LayerTag, pObject);
}

#pragma endregion

#pragma region RENDER_MANAGER

HRESULT CGameInstance::RenderManager_Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object)
{
	return m_pRenderManager->Add_RenderGroup(_RenderGroup, _Object);
}

#pragma endregion

#pragma region PLACE_MANAGER

inline HRESULT CGameInstance::PlaceManager_Change_Place(
	uint32_t PlaceID, 
	CPlace* pPlace)
{
	return m_pPlaceManager->Change_Place(PlaceID, pPlace);
}

#pragma endregion

#pragma region PROTOTYPE

inline HRESULT CGameInstance::Proto_Add_Prototype(
	uint32_t PlaceID,
	const _wstring ObjectTag,
	CObject* pPrototype)
{
	return m_pPrototype->Add_Prototype(PlaceID, ObjectTag, pPrototype);
}

inline CObject* CGameInstance::Proto_Get_Object_Clone(
	uint32_t PlaceID,
	const _wstring ObjectTag,
	void* pArg)
{
	return m_pPrototype->Get_Object_Clone(PlaceID, ObjectTag, pArg);
}

void CGameInstance::PipeLine_Set_Transform(D3DTS eMatrix, FXMMATRIX TransformMatrix)
{
	m_pPipeLine->Set_Transform(eMatrix, TransformMatrix);
}

HRESULT CGameInstance::PipeLine_Bind_PipeLine_Matrix(CShaderCom* pShader, const _char* pConstantName, D3DTS eMatrix)
{
	return m_pPipeLine->Bind_PipeLine_Matrix(pShader, pConstantName, eMatrix);
}

HRESULT CGameInstance::PipeLine_Bind_PipeLine_Inverse_Matrix(CShaderCom* pShader, const _char* pConstantName, D3DTS eMatrix)
{
	return m_pPipeLine->Bind_PipeLine_Inverse_Matrix(pShader, pConstantName, eMatrix);
}

//HRESULT CGameInstance::PipeLine_Bind_Camera_Pos(CShaderCom* pShader, const _char* pConstantName)
//{
//	return E_NOTIMPL;
//}

void CGameInstance::PipeLine_Calc_ProjMatrix(CCamera* pCamera)
{
	m_pPipeLine->Calc_ProjMatrix(pCamera);
}
#pragma endregion

#pragma region CAMERAMANAGER

HRESULT CGameInstance::CameraManager_Add_Camera(const _wstring CameraTag, CCamera* pCamera)
{
	return m_pCameraManager->Add_Camera(CameraTag, pCamera);
}

HRESULT CGameInstance::CameraManager_Bind_Matrix(
	CShaderCom* pShader, 
	const _char* ConstantName, 
	const _wstring CameraTag,
	D3DTS eMatrix)
{
	return m_pCameraManager->Bind_Matrix(
		pShader,
		ConstantName,
		CameraTag,
		eMatrix);
}
HRESULT CGameInstance::CameraManager_Bind_Camera_Pos(CShaderCom* pShader, const _char* pConstantName, const _wstring CameraTag)
{
	return m_pCameraManager->Bind_Camera_Pos(pShader, pConstantName, CameraTag);
}

const CCamera* CGameInstance::CameraManager_Get_Camera(const _wstring& strCameraTag)
{
	return m_pCameraManager->Get_Camera(strCameraTag);
}

void CGameInstance::CameraManager_FocusOnTarget(const _wstring& strCameraTag, FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio)
{
	m_pCameraManager->FocusOnTarget(strCameraTag, vTargetPos, fTargetFov, fRatio);
}


#pragma endregion

#pragma region INPUTDEVICE

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}


#pragma endregion

#pragma region LIGHTMANAGER

const LIGHT_DESC* CGameInstance::LightManager_Get_LightDesc(uint32_t iIndex)
{
	return m_pLightManager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::LightManager_Add_Light(const LIGHT_DESC& eLight)
{
	return m_pLightManager->Add_Light(eLight);
}


#pragma endregion

#pragma region MOUSEMANAGER

const POINT& CGameInstance::MouseManager_Get_MousePos()
{
	return m_pMouseManager->Get_MousePos();
}

const _bool CGameInstance::MouseManager_Get_isPressingLBT()
{
	return m_pMouseManager->Get_isPressingLBT();
}

const _bool CGameInstance::MouseManager_Get_isPressingRBT()
{
	return m_pMouseManager->Get_isPressingRBT();
}

const _bool CGameInstance::MouseManager_Get_isClickLBT()
{
	return m_pMouseManager->Get_isClickLBT();
}

const _bool CGameInstance::MouseManager_Get_isClickRBT()
{
	return m_pMouseManager->Get_isClickRBT();
}

const _bool CGameInstance::MouseManager_Get_isPressStartLBT()
{
	return m_pMouseManager->Get_isPressStartLBT();
}

const _bool CGameInstance::MouseManager_Get_isPressStartRBT()
{
	return m_pMouseManager->Get_isPressStartRBT();
}
#pragma endregion

#pragma region FILEMANAGER

HRESULT CGameInstance::FileManager_Read_File(const string& strFilePath, nlohmann::json& pOut)
{
	return m_pFileManager->Read_File(strFilePath, pOut);
}

HRESULT CGameInstance::FileManager_Write_File(
	const string& strFilePath, nlohmann::json& Data)
{
	return m_pFileManager->Write_File(strFilePath, Data);
}

const nlohmann::json& CGameInstance::FileManager_Get_Data(const string& strFilePath)
{
	return m_pFileManager->Get_Data(strFilePath);
}

void CGameInstance::FileManager_RoundFloatsInJson(nlohmann::json& j, int32_t n)
{
	m_pFileManager->RoundFloatsInJson(j, n);
}

#pragma endregion

#pragma region BUTTONMANAGER
HRESULT CGameInstance::ButtonManager_Add_Button(uint32_t iPlaceID, CButton* pButton)
{
	return m_pButtonManager->Add_Button(iPlaceID, pButton);
}

void CGameInstance::ButtonManager_All_Deactivate(uint32_t iPlaceID)
{
	m_pButtonManager->All_Deactivate(iPlaceID);
}

#pragma endregion

#pragma region ANIMATIONMANAGER

CSEAnimation* CGameInstance::AnimationManager_Get_Animation(const _string& strAnimationTag)
{
	return m_pAnimationManager->Get_Animation(strAnimationTag);
}

#pragma endregion






void CGameInstance::Clear_Engine()
{
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pButtonManager);
	Safe_Release(m_pCameraManager);
	Safe_Release(m_pPlaceManager);
	Safe_Release(m_pRenderManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pAnimationManager);
	Safe_Release(m_pPrototype);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pMouseManager);
	Safe_Release(m_pFileManager);

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	CBase::Free();
}
