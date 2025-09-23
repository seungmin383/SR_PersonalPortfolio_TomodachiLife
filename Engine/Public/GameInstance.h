#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CTimerManager;
class CGraphic_Device;
class CPlaceManager;
class CObjectManager;
class CRenderManager;
class CPrototype;

class CObject;
class CPlace;
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize();
	int32_t Update_Priority(const _float _fDT);
	int32_t Update(const _float _fDT);
	void	 Update_Late(const _float _fDT);

	void	Begin_Render();
	HRESULT Render();
	void	End_Render();

#pragma region TIMER_MANAGER
public:
	_float          TimerManager_Get_DeltaTime(const _tchar* pTimerTag);
	HRESULT         TimerManager_Ready_Timer(const _tchar* pTimerTag);
	void            TimerManager_Calc_DeltaTime(const _tchar* pTimerTag);
#pragma endregion

#pragma region GRAPHIC_DEVICE
public:
	HRESULT GraphicDevice_Initialize(HWND _hWnd,
		_uint _WinCX,
		_uint _WinCY,
		ID3D11Device** _ppDevice,
		ID3D11DeviceContext** _ppContext);

	HRESULT GraphicDevice_Clear_BackBuffer_View(const _float4* pClearColor);

	  /* 깊이버퍼 + 스텐실버퍼를 지운다. */
	HRESULT GraphicDevice_Clear_DepthStencil_View();

	  /* 후면 버퍼를 전면버퍼로 교체한다.(백버퍼를 화면에 직접 보여준다.) */
	HRESULT GraphicDevice_Present();

#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT ObjectManager_Add_Object_to_Layer(uint32_t _LayerID, const uint32_t _ObjectID);

#pragma endregion

#pragma region RENDER_MANAGER


#pragma endregion

#pragma region PLACE_MANAGER

	HRESULT PlaceManager_Go_Place(CPlace* _CPlace);
	HRESULT PlaceManager_Go_Back();

#pragma endregion

#pragma region PROTOTYPE
	HRESULT Proto_Add_Prototype(int32_t _ObjectID, CObject* _Origin);
	CObject* Proto_Get_Object_Clone(int32_t _ObjectID);
#pragma endregion

private:
	CTimerManager*		m_pTimerManager = { nullptr };
	CGraphic_Device*	m_pGraphicDevice = { nullptr };	
	CPlaceManager*		m_pPlaceManager = { nullptr };	
	CObjectManager*		m_pObjectManager = { nullptr };
	CRenderManager*		m_pRenderManager = { nullptr };
	CPrototype*			m_pPrototype = { nullptr };
public:
	virtual void Free()override;
};

NS_END