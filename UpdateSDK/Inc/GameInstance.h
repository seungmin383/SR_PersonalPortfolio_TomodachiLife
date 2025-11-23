#pragma once
#include "Base.h"
#include "Prototype.h"

NS_BEGIN(Engine)

class CAnimationManager;
class CButton;
class CButtonManager;
class CCamera;
class CCameraManager;
class CFileManager;
class CGraphic_Device;
class CInput_Device;
class CLightManager;
class CMouseManager;
class CObject;
class CObjectManager;
class CPipeLine;
class CPlace;
class CPlaceManager;
class CRenderManager;
class CTimerManager;
class CUI;
class CUIManager;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize(
		const ENGINE_DESC& Engine_desc,
		ID3D11Device** ppDevice,
		ID3D11DeviceContext** ppContext);

	int32_t Update_Priority(_float fDT);
	int32_t Update(_float fDT);
	void	Update_Late(_float fDT);

	void	Begin_Render();
	HRESULT Render_Top();
	HRESULT Render_Bottom();
	void	End_Render();

	HRESULT Clear_Resources(uint32_t PlaceID);
	_float Random(_float fMin, _float fMax);
public:
	#pragma region TIMER_MANAGER
	
	_float  TimerManager_Get_DeltaTime(const _tchar* pTimerTag);
	HRESULT TimerManager_Ready_Timer(const _tchar* pTimerTag);
	void    TimerManager_Calc_DeltaTime(const _tchar* pTimerTag);

#pragma endregion

	#pragma region GRAPHIC_DEVICE

	HRESULT GraphicDevice_Clear_BackBuffer_View(const _float4* pClearColor);

	  /* 깊이버퍼 + 스텐실버퍼를 지운다. */
	HRESULT GraphicDevice_Clear_DepthStencil_View();

	  /* 후면 버퍼를 전면버퍼로 교체한다.(백버퍼를 화면에 직접 보여준다.) */
	HRESULT GraphicDevice_Present();

	void GraphicDevice_Bind_Viewport(VIEWPORT_TYPE ViewportType);
	void GraphicDevice_Bind_RenderState(RENDER_STATE eRenderState);

	void GraphicDevice_AlphaBlend_On();
	void GraphicDevice_AlphaBlend_Off();
#pragma endregion

	#pragma region OBJECT_MANAGER

	HRESULT ObjectManager_Add_Object_in_Layer(
		uint32_t PlaceID,
		const _wstring LayerTag,
		CObject* pObject);

	HRESULT ObjectManager_Enable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag);
	HRESULT ObjectManager_Disable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag);

#pragma endregion

	#pragma region RENDER_MANAGER

	HRESULT RenderManager_Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object);
	
#pragma endregion

	#pragma region PLACE_MANAGER

	
	HRESULT PlaceManager_Change_Place(
		uint32_t PlaceID,
		CPlace* pPlace);

#pragma endregion

	#pragma region PROTOTYPE

	HRESULT Proto_Add_Prototype(
		uint32_t PlaceID, 
		const _wstring ObjectTag, 
		CObject* pPrototype);

	CObject* Proto_Get_Object_Clone(
		uint32_t PlaceID, 
		const _wstring ObjectTag, 
		void* pArg);

#pragma endregion

	#pragma region PIPELINE
	void PipeLine_Set_Transform(
		D3DTS eMatrix,
		FXMMATRIX TransformMatrix);

public:
	HRESULT PipeLine_Bind_PipeLine_Matrix(
		CShaderCom* pShader,
		const _char* pConstantName,
		D3DTS eMatrix);

	HRESULT PipeLine_Bind_PipeLine_Inverse_Matrix(
		CShaderCom* pShader,
		const _char* pConstantName,
		D3DTS eMatrix);

	//HRESULT PipeLine_Bind_Camera_Pos(
	//	CShaderCom* pShader,
	//	const _char* pConstantName);
	
	void PipeLine_Calc_ProjMatrix(CCamera* pCamera);

#pragma endregion

	#pragma region CAMERAMANAGER

	HRESULT CameraManager_Add_Camera(
		const _wstring CameraTag,
		CCamera* pCamera);

	HRESULT CameraManager_Bind_Matrix(
		CShaderCom* pShader,
		const _char* ConstantName,
		const _wstring CameraTag,
		D3DTS eMatrix);

	HRESULT CameraManager_Bind_Camera_Pos(
		CShaderCom* pShader,
		const _char* pConstantName,
		const _wstring CameraTag);

	CCamera* CameraManager_Get_Camera(const _wstring& strCameraTag);

	void CameraManager_FocusOnTarget(const _wstring& strCameraTag, FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio);
	
	void CameraManager_Reset_State(const _wstring& strCameraTag);

#pragma endregion

#pragma region INPUTDEVICE
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
#pragma endregion

#pragma region LIGHTMANAGER

	const LIGHT_DESC* LightManager_Get_LightDesc(uint32_t iIndex);
	HRESULT LightManager_Add_Light(const LIGHT_DESC& eLight);

#pragma endregion

#pragma region MOUSEMANAGER

	const POINT& MouseManager_Get_MousePos();

	const _bool MouseManager_Get_isPressingLBT();
	const _bool MouseManager_Get_isPressingRBT();
	const _bool MouseManager_Get_isClickLBT();
	const _bool MouseManager_Get_isClickRBT();
	const _bool MouseManager_Get_isPressStartLBT();
	const _bool MouseManager_Get_isPressStartRBT();

#pragma endregion

#pragma region FILEMANAGER
public:
	HRESULT FileManager_Read_File(
		const string& strFilePath,
		nlohmann::json& pOut);

	HRESULT FileManager_Write_File(
		const string& strFilePath,
		nlohmann::json& Data);

	const nlohmann::json& FileManager_Get_Data(const string& strFilePath);

	void FileManager_RoundFloatsInJson(nlohmann::json& j, int32_t n = 6);

#pragma endregion

#pragma region BUTTONMANAGER
	HRESULT ButtonManager_Add_Button(uint32_t iPlaceID, CButton* pButton);

	void ButtonManager_All_Deactivate(uint32_t iPlaceID);
#pragma endregion

#pragma region ANIMATIONMANAGER
	CSEAnimation* AnimationManager_Get_Animation(const _string& strAnimationTag);

#pragma endregion

#pragma region UIMANAGER
	HRESULT UIManager_Add_UI(uint32_t iUIGroupIndex, const _wstring& strUITag, CUI* pUI);
	void UIManager_Change_UIGroup(uint32_t iNextGroupIndex, _bool isLoading = FALSE);
	void UIManager_Change_Place(uint32_t iInitGroupIndex);

	_float  UIManager_Get_FadeForce();
#pragma endregion


private:
	CTimerManager*		m_pTimerManager = { nullptr };
	CInput_Device*      m_pInput_Device = { nullptr };
	CGraphic_Device*	m_pGraphicDevice = { nullptr };	
	CPlaceManager*		m_pPlaceManager = { nullptr };	
	CObjectManager*		m_pObjectManager = { nullptr };
	CRenderManager*		m_pRenderManager = { nullptr };
	CPrototype*			m_pPrototype	= { nullptr };
	CPipeLine*			m_pPipeLine = { nullptr };
	CCameraManager*		m_pCameraManager = { nullptr };
	CLightManager*		m_pLightManager = { nullptr };
	CMouseManager*		m_pMouseManager = { nullptr };
	CFileManager*		m_pFileManager = { nullptr };
	CButtonManager*		m_pButtonManager = { nullptr };
	CAnimationManager*	m_pAnimationManager = { nullptr };
	CUIManager*			m_pUIManager = { nullptr };

public:
	void Clear_Engine();
	virtual void Free()override;
};

NS_END