#include "GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Place_Manager.h"
#include "Render_Manager.h"
#include "Object_Manager.h"
#include "Prototype.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize()
{
	//TimerManager
	{
		m_pTimerManager = CTimerManager::Create();
		if (nullptr == m_pTimerManager)
		{
			MSG_BOX("ErrCode : m_pTimerManager Create Failed");
			Safe_Release(m_pTimerManager);
			return E_FAIL;
		}
	}

	//PlaceManager
	{
		m_pPlaceManager = CPlaceManager::Create();
		if (nullptr == m_pPlaceManager)
		{
			MSG_BOX("ErrCode : m_pPlaceManager Create Failed");
			Safe_Release(m_pPlaceManager);
			return E_FAIL;
		}
	}

	//CRenderManager
	{
		m_pRenderManager = CRenderManager::Create();
		if (nullptr == m_pRenderManager)
		{
			MSG_BOX("ErrCode : m_pRenderManager Create Failed");
			Safe_Release(m_pRenderManager);
			return E_FAIL;
		}
	}

	//CObjectManager
	{
		m_pObjectManager = CObjectManager::Create();
		if (nullptr == m_pObjectManager)
		{
			MSG_BOX("ErrCode : m_pObjectManager Create Failed");
			Safe_Release(m_pObjectManager);
			return E_FAIL;
		}
	}

	return S_OK;
}

int32_t CGameInstance::Update_Priority(const _float _fDT)
{
	return m_pObjectManager->Update_Priority(_fDT);
}

int32_t CGameInstance::Update(const _float _fDT)
{
	//게임 씬 매니저 업데이트			//주로 디버그용
	m_pPlaceManager->Update(_fDT);
	//게임 오브젝트 매니저 업데이트		//진짜 업데이트
	
	return m_pObjectManager->Update(_fDT);
}

void CGameInstance::Update_Late(const _float _fDT)
{
	m_pObjectManager->Update_Late(_fDT);
}

void CGameInstance::Begin_Render()
{
	const _float4 m_fColor = { 255.f, 0.f, 255.f, 0.f };

	if (FAILED(GraphicDevice_Clear_BackBuffer_View(&m_fColor)))
		return;

	GraphicDevice_Clear_DepthStencil_View();
}

HRESULT CGameInstance::Render()
{
	//렌더 매니저 렌더
	if (FAILED(m_pRenderManager->Render()))
	{
		MSG_BOX("ErrCode : RenderManager Render Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGameInstance::End_Render()
{
	GraphicDevice_Present();
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

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::ObjectManager_Add_Object_to_Layer(uint32_t _LayerID, const uint32_t _ObjectID)
{
	return m_pObjectManager->Add_Object_to_Layer(_LayerID, _ObjectID);
}
#pragma endregion

#pragma region RENDER_MANAGER

#pragma endregion

#pragma region PLACE_MANAGER

HRESULT CGameInstance::PlaceManager_Go_Place(uint32_t _NextPlaceID, CPlace* _CPlace)
{
	return m_pPlaceManager->Go_Place(_NextPlaceID, _CPlace);
}

HRESULT CGameInstance::PlaceManager_Go_Back()
{
	return m_pPlaceManager->Go_Back();
}

#pragma endregion

#pragma region PROTOTYPE

HRESULT CGameInstance::Proto_Add_Prototype(int32_t _ObjectID, CObject* _Origin)
{
	return m_pPrototype->Add_Prototype(_ObjectID, _Origin);
}
CObject* CGameInstance::Proto_Get_Object_Clone(int32_t _ObjectID)
{
	return m_pPrototype->Get_Object_Clone(_ObjectID);
}

#pragma endregion

void CGameInstance::Clear_Engine()
{
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pPrototype);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pRenderManager);
	Safe_Release(m_pPlaceManager);
	Safe_Release(m_pGraphicDevice);

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	CBase::Free();
}
