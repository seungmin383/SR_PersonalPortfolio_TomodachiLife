#include "GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize()
{
	m_pTimerManager = CTimerManager::Create();

	if (nullptr == m_pTimerManager)
	{
		MSG_BOX("ErrCode : m_pTimerManager Create Failed");
		Safe_Release(m_pTimerManager);
		return E_FAIL;
	}

	return S_OK;
}

int32_t CGameInstance::Update(const _float fTimeDelta)
{
	return 0;
}

void CGameInstance::Begin_Render()
{
	const _float4 fColor = { 255.f, 0.f, 255.f, 0.f };

	if (FAILED(GraphicDevice_Clear_BackBuffer_View(&fColor)))
		return;

	GraphicDevice_Clear_DepthStencil_View();
}

HRESULT CGameInstance::Render()
{
	return S_OK;
}

void CGameInstance::End_Render()
{
	GraphicDevice_Present();
}

#pragma region TIMER_MANAGER

inline _float CGameInstance::TimerManager_Get_TimeDelta(const _tchar* pTimerTag)
{
	return m_pTimerManager->Get_TimeDelta(pTimerTag);
}

inline HRESULT CGameInstance::TimerManager_Ready_Timer(const _tchar* pTimerTag)
{
	return m_pTimerManager->Ready_Timer(pTimerTag);
}

inline void CGameInstance::TimerManager_Calc_TimeDelta(const _tchar* pTimerTag)
{
	m_pTimerManager->Calc_TimeDelta(pTimerTag);
}

#pragma endregion

#pragma region GRAPHIC_DEVICE

inline HRESULT CGameInstance::GraphicDevice_Initialize(HWND _hWnd,
	_uint _WinCX,
	_uint _WinCY,
	ID3D11Device** _ppDevice,
	ID3D11DeviceContext** _ppContext)
{
	m_pGraphicDevice = CGraphic_Device::Create(_hWnd, WINMODE::WIN, _WinCX, _WinCY, _ppDevice, _ppContext);

	if (nullptr == m_pGraphicDevice)
	{
		MSG_BOX("ErrCode : m_pGraphicDevice Create Failed");
		Safe_Release(m_pGraphicDevice);
		return E_FAIL;
	}
	return S_OK;
}

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

#pragma endregion

void CGameInstance::Free()
{
	Safe_Release(m_pTimerManager);
	m_pGraphicDevice->Free();
	Safe_Release(m_pGraphicDevice);
	CBase::Free();
}
