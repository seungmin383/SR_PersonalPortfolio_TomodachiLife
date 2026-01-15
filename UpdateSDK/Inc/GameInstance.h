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
class CCollisionManager;
class CCutScene;
class CActorManager;
class CFontManager;
class CTargetManager;
class CSoundManager;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize(	const ENGINE_DESC& Engine_desc,	ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	int32_t Update_Priority(_float fDT);
	int32_t Update(_float fDT);
	void	Update_Late(_float fDT);

	void	Begin_Render();
	HRESULT Render_Top();
	HRESULT Render_Bottom();
	void	End_Render();

	HRESULT Clear_Resources(uint32_t PlaceID);
	_float Random(_float fMin, _float fMax);
	int32_t Real_Random(int32_t iMin, int32_t iMax);

	void Get_String(wchar_t ch);

private:
	mt19937 m_rng = {};

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

#pragma endregion

	#pragma region OBJECT_MANAGER

	HRESULT ObjectManager_Add_Object_in_Layer(
		uint32_t PlaceID,
		const _wstring LayerTag,
		CObject* pObject);

	HRESULT ObjectManager_Enable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag);
	HRESULT ObjectManager_Disable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag);
	class CLayer* ObjectManager_Find_Layer(uint32_t iPlaceID, const _wstring& strLayerTag);

#pragma endregion

	#pragma region RENDER_MANAGER

	HRESULT RenderManager_Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object);
	
#pragma endregion

	#pragma region PLACE_MANAGER

	
	HRESULT PlaceManager_Change_Place(uint32_t PlaceID,CPlace* pPlace);

	uint32_t PlaceManager_Get_CurPlaceID()const;

#pragma endregion

	#pragma region PROTOTYPE

	HRESULT Proto_Add_Prototype(uint32_t PlaceID, const _wstring ObjectTag, CObject* pPrototype);

	CObject* Proto_Get_Object_Clone(uint32_t PlaceID, const _wstring ObjectTag, void* pArg);

#pragma endregion

	#pragma region PIPELINE
	void PipeLine_Set_Transform(D3DTS eMatrix,FXMMATRIX TransformMatrix);

public:
	HRESULT PipeLine_Bind_PipeLine_Matrix(CShaderCom* pShader,const _char* pConstantName,D3DTS eMatrix);

	HRESULT PipeLine_Bind_PipeLine_Inverse_Matrix(CShaderCom* pShader,	const _char* pConstantName,
		D3DTS eMatrix);

	//HRESULT PipeLine_Bind_Camera_Pos(
	//	CShaderCom* pShader,
	//	const _char* pConstantName);
	
	void PipeLine_Calc_ProjMatrix(CCamera* pCamera);

#pragma endregion

	#pragma region CAMERAMANAGER

	HRESULT CameraManager_Add_Camera(const _wstring CameraTag,CCamera* pCamera);

	HRESULT CameraManager_Bind_Matrix(CShaderCom* pShader,const _char* ConstantName,const _wstring CameraTag,D3DTS eMatrix);

	HRESULT CameraManager_Bind_Camera_Pos(CShaderCom* pShader, const _char* pConstantName,const _wstring CameraTag);

	HRESULT CameraManager_Bind_Matrix_Inv(CShaderCom* pShader, const _char* ConstantName,const _wstring CameraTag, D3DTS eMatrix);

	CCamera* CameraManager_Get_Camera(const _wstring& strCameraTag);

	void CameraManager_FocusOnTarget(const _wstring& strCameraTag, FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio);
	
	void CameraManager_Reset_State(const _wstring& strCameraTag);

	void CameraManager_Set_Position(const _wstring& strCameraTag, FXMVECTOR vPos);

#pragma endregion

#pragma region INPUTDEVICE
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
#pragma endregion

#pragma region LIGHTMANAGER

	const LIGHT_DESC* LightManager_Get_LightDesc(uint32_t iIndex);
	HRESULT LightManager_Add_Light(const LIGHT_DESC& eLight);

	void LightManager_Render_Light(CShaderCom* pShader, CRectBufferCom* pBuffer);

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
	HRESULT FileManager_Read_File(const string& strFilePath,nlohmann::json& pOut);

	HRESULT FileManager_Write_File(const string& strFilePath,nlohmann::json& Data);

	const nlohmann::json& FileManager_Get_Data(const string& strFilePath);

	void FileManager_RoundFloatsInJson(nlohmann::json& j, int32_t n = 6);

#pragma endregion

#pragma region BUTTONMANAGER
	HRESULT		ButtonManager_Add_Button(uint32_t iPlaceID, const _wstring& strLayerTag, CButton* pButton);

	HRESULT		ButtonManager_Layer_Deactivate(uint32_t iPlaceID, const _wstring& strLayerTag);
	HRESULT		ButtonManager_All_Deactivate(uint32_t iPlaceID);

	void		ButtonManager_Set_CurButton(CButton* pButton);
	CButton*	ButtonManager_Get_CurButton();

	void		ButtonManager_Set_OnText(_bool is);

	uint32_t	ButtonManager_Get_BuildingNameTextureIndex();
	void		ButtonManager_Set_BuildingNameTextureIndex(uint32_t i);

	_bool		ButtonManager_Get_HasSelectedPlaceMoveButton();
	void		ButtonManager_Set_HasSelectedPlaceMoveButton(_bool isSelected);

	_bool		ButtonManager_Get_HasSelectedCategoryButton() const;
	void		ButtonManager_Set_HasSelectedCategoryButton(_bool isSelected);

	void ButtonManager_Clear_Text();
	_wstring ButtonManager_Get_Text();

#pragma endregion

#pragma region ANIMATIONMANAGER
	CSEAnimation* AnimationManager_Find_Animation(const _wstring& strAnimationTag);
	vector<_wstring>* AnimationManager_Find_AnimationMacro(const _wstring& strMacroTag);
	vector<_float>* AnimationManager_Find_AnimationSpeeds(const _wstring& strMacroTag);
	vector<uint32_t>* AnimationManager_Find_AnimationLoopTime(const _wstring& strMacroTag);

#pragma endregion

#pragma region UIMANAGER
	HRESULT UIManager_Add_UI(uint32_t iUIGroupIndex, const _wstring& strUITag, CUI* pUI);
	void UIManager_Change_UIGroup(uint32_t iNextGroupIndex, _bool isLoading = FALSE);
	void UIManager_Change_Place(uint32_t iInitGroupIndex);

	_float  UIManager_Get_FadeForce();

	_bool UIManager_Get_isEnable() const;
	void UIManager_Set_isEnable(_bool is);

#pragma endregion

#pragma region COLLISIONMANAGER
	HRESULT CollsionManager_Add_Collider(uint32_t iPlaceID, COLLISION eCollisionType, CCollider* pCollider);

#pragma endregion

#pragma region ACTORMANGER
	void ActorManager_Set_IsBuildingUnlock(uint32_t i, _bool isUnlock);
	_bool ActorManager_Get_IsBuildingUnlock(uint32_t i) const;
#pragma endregion

#pragma region FONTMANAGER

	HRESULT FontManager_Add_Font(const _wstring& strFontTag, const _wstring& strFontFilePath);
	HRESULT FontManager_Draw_Text(const _wstring& strFontTag, const _wstring& strText, const _float2& vPos, FXMVECTOR vColor);
	const XMFLOAT4& FontManager_Measure_Text(
		const _wstring& strFontTag,
		const _wstring& strText);
#pragma endregion

#pragma region TARGETMANAGER
	HRESULT TargetManager_Add_RenderTarget(const _wstring& strTargetTag, uint32_t iWidth, uint32_t iHeight, DXGI_FORMAT ePixelFormat, const XMFLOAT4& vClearColor);
	HRESULT TargetManager_Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT TargetManager_Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT TargetManager_End_MRT();
	HRESULT TargetManager_Bind_ShaderResource(const _wstring& strTargetTag, CShaderCom* pShader, const _char* pConstantName);
	void TargetManager_Copy_Resource(const _wstring& strTargetTag, ID3D11Resource* pDst);

#ifdef _DEBUG
	HRESULT TargetManager_Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fCX, _float fCY);
	HRESULT TargetManager_Render(const _wstring& strMRTTag, CShaderCom* pShader, CRectBufferCom* pBuffer);
#endif


#pragma region SOUNDMANAGER
	HRESULT SoundManager_Load_Sound(const _wstring& SoundTag, const _string& SoundFilePath, _bool isBGM = FALSE, _bool isLoop = FALSE);
	HRESULT SoundManager_Begin_BGM(const _wstring& SoundTag, _float fVolume = 1.f);
	HRESULT SoundManager_Begin_Sound(const _wstring& SoundTag, SOUND_CHANNEL eChannel, _float fVolume = 1.f);
	HRESULT SoundManager_Stop_Sound(SOUND_CHANNEL eChannel);
	HRESULT SoundManager_Stop_AllSound();
	HRESULT SoundManager_Begin_Voice(const _wstring& strText, _float fPitch, _float fVolume = 1.f);
	HRESULT SoundManager_Begin_Girl_Voice(const _wstring& strText, _float fPitch, _float fSpeed, _float fVolume = 1.f);
#pragma endregion


private:
	CTimerManager*		m_pTimerManager		= { nullptr };
	CInput_Device*      m_pInput_Device		= { nullptr };
	CGraphic_Device*	m_pGraphicDevice	= { nullptr };	
	CPlaceManager*		m_pPlaceManager		= { nullptr };	
	CObjectManager*		m_pObjectManager	= { nullptr };
	CRenderManager*		m_pRenderManager	= { nullptr };
	CPrototype*			m_pPrototype		= { nullptr };
	CPipeLine*			m_pPipeLine			= { nullptr };
	CCameraManager*		m_pCameraManager	= { nullptr };
	CLightManager*		m_pLightManager		= { nullptr };
	CMouseManager*		m_pMouseManager		= { nullptr };
	CFileManager*		m_pFileManager		= { nullptr };
	CButtonManager*		m_pButtonManager	= { nullptr };
	CAnimationManager*	m_pAnimationManager = { nullptr };
	CUIManager*			m_pUIManager		= { nullptr };
	CCollisionManager*	m_pCollisionManager = { nullptr };
	CActorManager*		m_pActorManager		= { nullptr };
	CFontManager*		m_pFontManager		= { nullptr };
	CTargetManager*		m_pTargetManager	= { nullptr };
	CSoundManager*		m_pSoundManager = { nullptr };

public:
	void Clear_Engine();
	virtual void Free()override;
};

NS_END