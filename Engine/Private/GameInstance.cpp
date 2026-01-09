#include "AnimationManager.h"
#include "Button.h"
#include "ButtonManager.h"
#include "Camera.h"
#include "CameraManager.h"
#include "FileManager.h"
#include "GameInstance.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "LightManager.h"
#include "MouseManager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Place_Manager.h"
#include "FontManager.h"
#include "Render_Manager.h"
#include "Timer_Manager.h"
#include "UI.h"
#include "UIManager.h"
#include "CollisionManager.h"
#include "ActorManager.h"
#include "Target_Manager.h"

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

	// TargetManager
	{
		m_pTargetManager = CTargetManager::Create(*ppDevice, *ppContext);
		if (nullptr == m_pTargetManager)
		{
			MSG_BOX("ErrCode : TargetManager Create Failed");
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

	// LightManager
	{
		m_pLightManager = CLightManager::Create();
		if (nullptr == m_pCameraManager) {
			MSG_BOX("ErrCode : CLightManager Create Failed");
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
		if (nullptr == m_pAnimationManager) {
			MSG_BOX("ErrCode : AnimationManager Create Failed");
			return E_FAIL;
		}
	}

	// UIManager
	{
		m_pUIManager = CUIManager::Create(*ppDevice, *ppContext);
		if (nullptr == m_pUIManager) {
			MSG_BOX("ErrCode : UIManager Create Failed");
			return E_FAIL;
		}
	}

	// CollisionManager
	{
		m_pCollisionManager = CCollisionManager::Create(Engine_desc.PlaceCount);
		if (nullptr == m_pCollisionManager){
			MSG_BOX("ErrCode : CollisionManager Create Failed");
			return E_FAIL;
		}
	}

	// ActorManager
	{
		m_pActorManager = CActorManager::Create();
		if(nullptr == m_pActorManager)
		{
			MSG_BOX("ErrCode : ActorManager Create Failed");
			return E_FAIL;
		}
	}

	// FontManager
	{
		m_pFontManager = CFontManager::Create(*ppDevice, *ppContext);
		if (nullptr == m_pActorManager)
		{
			MSG_BOX("ErrCode : FontManager Create Failed");
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
	m_pUIManager->UIGroup_Update_Priority(fDT);

	return 0;
}

int32_t CGameInstance::Update(_float fDT)
{
	//게임 씬 매니저 업데이트			//주로 디버그용
	m_pPlaceManager->Update(fDT);

	//게임 오브젝트 매니저 업데이트		//진짜 업데이트
	m_pObjectManager->Update(fDT);
	m_pCollisionManager->Update(fDT);
	
	m_pCameraManager->Update(fDT);
	m_pUIManager->UIGroup_Update(fDT);
	return 	0;
}

void CGameInstance::Update_Late( _float fDT)
{
	m_pObjectManager->Update_Late(fDT);	
	m_pCameraManager->Update_Late(fDT);
	m_pUIManager->UIGroup_Update_Late(fDT);
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
	if (FAILED(m_pCollisionManager->Clear_Collides(PlaceID)))
		return E_FAIL;
	return S_OK;
}

_float CGameInstance::Random(_float fMin, _float fMax)
{
	_float		fRandNormal = static_cast<_float>(rand()) / RAND_MAX;

	return (fMax - fMin) * fRandNormal + fMin;
}

void CGameInstance::Get_String(wchar_t ch)
{
	if (!m_pButtonManager->Get_OnText())
		return;

	m_pButtonManager->Append_Char(ch);
}

#pragma region TIMER_MANAGER

 _float CGameInstance::TimerManager_Get_DeltaTime(const _tchar* pTimerTag)
{
	return m_pTimerManager->Get_DeltaTime(pTimerTag);
}

 HRESULT CGameInstance::TimerManager_Ready_Timer(const _tchar* pTimerTag)
{
	return m_pTimerManager->Ready_Timer(pTimerTag);
}

 void CGameInstance::TimerManager_Calc_DeltaTime(const _tchar* pTimerTag)
{
	m_pTimerManager->Calc_DeltaTime(pTimerTag);
}

#pragma endregion

#pragma region GRAPHIC_DEVICE

 HRESULT CGameInstance::GraphicDevice_Clear_BackBuffer_View(const _float4* pClearColor)
{
	return m_pGraphicDevice->Clear_BackBuffer_View(pClearColor);
}

 HRESULT CGameInstance::GraphicDevice_Clear_DepthStencil_View()
{
	return m_pGraphicDevice->Clear_DepthStencil_View();
}

 HRESULT CGameInstance::GraphicDevice_Present()
{
	return m_pGraphicDevice->Present();
}

void CGameInstance::GraphicDevice_Bind_Viewport(VIEWPORT_TYPE ViewportType)
{
	m_pGraphicDevice->Bind_Viewport(ViewportType);
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

HRESULT CGameInstance::ObjectManager_Enable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag)
{
	return m_pObjectManager->Enable_Layer(iPlaceID, strLayerTag);
}

HRESULT CGameInstance::ObjectManager_Disable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag)
{
	return m_pObjectManager->Disable_Layer(iPlaceID, strLayerTag);
}

CLayer* CGameInstance::ObjectManager_Find_Layer(uint32_t iPlaceID, const _wstring& strLayerTag)
{
	return m_pObjectManager->Find_Layer(iPlaceID, strLayerTag);
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

uint32_t CGameInstance::PlaceManager_Get_CurPlaceID() const
{
	return m_pPlaceManager->Get_CurPlaceID();
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

HRESULT CGameInstance::CameraManager_Bind_Matrix_Inv(CShaderCom* pShader, const _char* ConstantName, const _wstring CameraTag, D3DTS eMatrix)
{
	return m_pCameraManager->Bind_Matrix_Inv(pShader, ConstantName, CameraTag, eMatrix);
}

CCamera* CGameInstance::CameraManager_Get_Camera(const _wstring& strCameraTag)
{
	return m_pCameraManager->Get_Camera(strCameraTag);
}

void CGameInstance::CameraManager_FocusOnTarget(const _wstring& strCameraTag, FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio)
{
	m_pCameraManager->FocusOnTarget(strCameraTag, vTargetPos, fTargetFov, fRatio);
}

void CGameInstance::CameraManager_Reset_State(const _wstring& strCameraTag)
{
	m_pCameraManager->Reset_State(strCameraTag);
}

void CGameInstance::CameraManager_Set_Position(const _wstring& strCameraTag, FXMVECTOR vPos)
{
	m_pCameraManager->Set_Position(strCameraTag, vPos);
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

void CGameInstance::LightManager_Render_Light(CShaderCom* pShader, CRectBufferCom* pBuffer)
{
	m_pLightManager->Render_Light(pShader, pBuffer);
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
HRESULT CGameInstance::ButtonManager_Add_Button(uint32_t iPlaceID, const _wstring& strLayerTag, CButton* pButton)
{
	return m_pButtonManager->Add_Button(iPlaceID, strLayerTag, pButton);
}

HRESULT CGameInstance::ButtonManager_Layer_Deactivate(uint32_t iPlaceID, const _wstring& strLayerTag)
{
	return m_pButtonManager->Layer_Deactivate(iPlaceID, strLayerTag);
}

HRESULT CGameInstance::ButtonManager_All_Deactivate(uint32_t iPlaceID)
{
	return m_pButtonManager->All_Deactive(iPlaceID);
}

void CGameInstance::ButtonManager_Set_CurButton(CButton* pButton)
{
	m_pButtonManager->Set_CurButton(pButton);
}

CButton* CGameInstance::ButtonManager_Get_CurButton()
{
	return m_pButtonManager->Get_CurButton();
}

void CGameInstance::ButtonManager_Set_OnText(_bool is)
{
	m_pButtonManager->Set_OnText(is);
}

void CGameInstance::ButtonManager_Set_BuildingNameTextureIndex(uint32_t i)
{
	m_pButtonManager->Set_BuildingNameTextureIndex(i);
}

uint32_t CGameInstance::ButtonManager_Get_BuildingNameTextureIndex()
{
	return m_pButtonManager->Get_BuildingNameTextureIndex();
}

void CGameInstance::ButtonManager_Set_HasSelectedPlaceMoveButton(_bool isSelected)
{
	m_pButtonManager->Set_HasSelectedPlaceMoveButton(isSelected);
}

_bool CGameInstance::ButtonManager_Get_HasSelectedPlaceMoveButton()
{
	return m_pButtonManager->Get_HasSelectedPlaceMoveButton();
}

void CGameInstance::ButtonManager_Set_HasSelectedCategoryButton(_bool isSelected)
{
	m_pButtonManager->Set_HasSelectedCategoryButton(isSelected);
}

_bool CGameInstance::ButtonManager_Get_HasSelectedCategoryButton() const
{
	return m_pButtonManager->Get_HasSelectedCategoryButton();
}

_wstring CGameInstance::ButtonManager_Get_Text()
{
	return m_pButtonManager->Get_Text();
}

#pragma endregion

#pragma region ANIMATIONMANAGER

CSEAnimation* CGameInstance::AnimationManager_Find_Animation(const _wstring& strAnimationTag)
{
	return m_pAnimationManager->Find_Animation(strAnimationTag);
}

vector<_wstring>* CGameInstance::AnimationManager_Find_AnimationMacro(const _wstring& strMacroTag)
{
	return m_pAnimationManager->Find_AnimationMacro(strMacroTag);
}

vector<_float>* CGameInstance::AnimationManager_Find_AnimationSpeeds(const _wstring& strMacroTag)
{
	return m_pAnimationManager->Find_AnimationSpeeds(strMacroTag);
}

vector<uint32_t>* CGameInstance::AnimationManager_Find_AnimationLoopTime(const _wstring& strMacroTag)
{
	return m_pAnimationManager->Find_AnimationLoopTime(strMacroTag);
}

#pragma endregion

#pragma region UIMANAGER

HRESULT CGameInstance::UIManager_Add_UI(uint32_t iUIGroupIndex, const _wstring& strUITag, CUI* pUI)
{
	return m_pUIManager->Add_UI(iUIGroupIndex, strUITag, pUI);
}

void CGameInstance::UIManager_Change_UIGroup(uint32_t iNextGroupIndex, _bool isLoading)
{
	m_pUIManager->Change_UIGroup(iNextGroupIndex, isLoading);
}

void CGameInstance::UIManager_Change_Place(uint32_t iInitGroupIndex)
{
	m_pUIManager->Change_Place(iInitGroupIndex);
}

_float CGameInstance::UIManager_Get_FadeForce()
{
	return m_pUIManager->Get_FadeForce();
}

_bool CGameInstance::UIManager_Get_isEnable() const
{
	return m_pUIManager->Get_isEnable();
}

void CGameInstance::UIManager_Set_isEnable(_bool is)
{
	m_pUIManager->Set_isEnable(is);
}

#pragma endregion

#pragma region COLLISIONMANAGER

HRESULT CGameInstance::CollsionManager_Add_Collider(uint32_t iPlaceID, COLLISION eCollisionType, CCollider* pCollider)
{
	return m_pCollisionManager->Add_Collider(iPlaceID, eCollisionType, pCollider);
}
#pragma endregion

#pragma region ACTORMANAGER

void CGameInstance::ActorManager_Set_IsBuildingUnlock(uint32_t i, _bool isUnlock)
{
	m_pActorManager->Set_IsBuildingUnlock(i, isUnlock);
}

_bool CGameInstance::ActorManager_Get_IsBuildingUnlock(uint32_t i) const
{
	return m_pActorManager->Get_IsBuildingUnlock(i);
}
#pragma endregion




#pragma region FONTMANAGER
HRESULT CGameInstance::FontManager_Add_Font(const _wstring& strFontTag, const _wstring& strFontFilePath)
{
	return m_pFontManager->Add_Font(strFontTag, strFontFilePath);
}

HRESULT CGameInstance::FontManager_Draw_Text(const _wstring& strFontTag, const _wstring& strText, const _float2& vPos, FXMVECTOR vColor)
{
	return m_pFontManager->Draw_Text(strFontTag, strText, vPos, vColor);
}

const XMFLOAT4& CGameInstance::FontManager_Measure_Text(const _wstring& strFontTag, const _wstring& strText)
{
	return m_pFontManager->Measure_Text(strFontTag, strText);
}
#pragma endregion

#pragma region TARGETMANAGER

HRESULT CGameInstance::TargetManager_Add_RenderTarget(const _wstring& strTargetTag, uint32_t iWidth, uint32_t iHeight, DXGI_FORMAT ePixelFormat, const XMFLOAT4& vClearColor)
{
	return m_pTargetManager->Add_RenderTarget(strTargetTag, XMUINT2{iWidth, iHeight}, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::TargetManager_Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTargetManager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::TargetManager_Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	return m_pTargetManager->Begin_MRT(strMRTTag, pDSV);
}

HRESULT CGameInstance::TargetManager_End_MRT()
{
	return m_pTargetManager->End_MRT();
}

HRESULT CGameInstance::TargetManager_Bind_ShaderResource(const _wstring& strTargetTag, CShaderCom* pShader, const _char* pConstantName)
{
	return m_pTargetManager->Bind_ShaderResource(strTargetTag, pShader, pConstantName);
}

void CGameInstance::TargetManager_Copy_Resource(const _wstring& strTargetTag, ID3D11Resource* pDst)
{
	m_pTargetManager->Copy_Resource(strTargetTag, pDst);
}

HRESULT CGameInstance::TargetManager_Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fCX, _float fCY)
{
	return m_pTargetManager->Ready_Debug(strTargetTag, fX, fY, fCX, fCY);
}

HRESULT CGameInstance::TargetManager_Render(const _wstring& strMRTTag, CShaderCom* pShader, CRectBufferCom* pBuffer)
{
	return m_pTargetManager->Render(strMRTTag, pShader, pBuffer);
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
	Safe_Release(m_pLightManager);
	Safe_Release(m_pActorManager);
	Safe_Release(m_pRenderManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pAnimationManager);
	Safe_Release(m_pPrototype);
	Safe_Release(m_pMouseManager);
	Safe_Release(m_pFontManager);
	Safe_Release(m_pUIManager);
	Safe_Release(m_pFileManager);
	Safe_Release(m_pCollisionManager);
	Safe_Release(m_pTargetManager);
	Safe_Release(m_pGraphicDevice);

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	CBase::Free();
}
