#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CTimerManager;
class CGraphic_Device;
class CRenderManager;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize();
	int32_t Update(const _float fTimeDelta);

	void Begin_Render();
	HRESULT Render();
	void	End_Render();

#pragma region TIMER_MANAGER
public:
	_float          TimerManager_Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT         TimerManager_Ready_Timer(const _tchar* pTimerTag);
	void            TimerManager_Calc_TimeDelta(const _tchar* pTimerTag);
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

private:
	CTimerManager* m_pTimerManager = { nullptr };
	CGraphic_Device* m_pGraphicDevice = { nullptr };
	CRenderManager* m_pRenderManager = { nullptr };

public:
	virtual void Free()override;
};

NS_END