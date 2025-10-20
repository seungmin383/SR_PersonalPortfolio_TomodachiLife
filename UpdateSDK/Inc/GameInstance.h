#pragma once
#include "Base.h"
#include "Prototype.h"

NS_BEGIN(Engine)
class CTimerManager;
class CGraphic_Device;
class CPlaceManager;
class CObjectManager;
class CRenderManager;
class CPipeLine;

class CObject;
class CPlace;

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

#pragma endregion

	#pragma region RENDER_MANAGER

	HRESULT RenderManager_Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object);
	
#pragma endregion

	#pragma region PLACE_MANAGER

	HRESULT PlaceManager_Change_Place(
		uint32_t PlaceID,
		RETAIN_POLICY Policy,
		CPlace* pPlace);

	HRESULT PlaceManager_Return_Place();

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

	HRESULT PipeLine_Bind_Camera_Pos(
		CShaderCom* pShader,
		const _char* pConstantName);

#pragma endregion


private:
	CTimerManager*		m_pTimerManager = { nullptr };
	CGraphic_Device*	m_pGraphicDevice = { nullptr };	
	CPlaceManager*		m_pPlaceManager = { nullptr };	
	CObjectManager*		m_pObjectManager = { nullptr };
	CRenderManager*		m_pRenderManager = { nullptr };
	CPrototype*			m_pPrototype	= { nullptr };
	CPipeLine*			m_pPipeLine = { nullptr };

public:
	void Clear_Engine();
	virtual void Free()override;
};

NS_END