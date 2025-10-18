#include "GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Place_Manager.h"
#include "Render_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"

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
			return E_FAIL;
	}

	//PlaceManager
	{
		m_pPlaceManager = CPlaceManager::Create();
		if (nullptr == m_pPlaceManager)
			return E_FAIL;
	}

	//CObjectManager
	{
		m_pObjectManager = CObjectManager::Create(Engine_desc.PlaceCount);
		if (nullptr == m_pObjectManager)
			return E_FAIL;
	}

	//Prototype
	{
		m_pPrototype = CPrototype::Create(
			*ppDevice, 
			*ppContext, 
			Engine_desc.PlaceCount);

		if (nullptr == m_pPrototype)
			return E_FAIL;
	}

	//RenderManager
	{
		m_pRenderManager = CRenderManager::Create(*ppDevice, *ppContext);
		if (nullptr == m_pRenderManager)
			return E_FAIL;
	}

	// PipeLine
	{
		m_pPipeLine = CPipeLine::Create();
		if (nullptr == m_pPipeLine)
			return E_FAIL;
	}
	return S_OK;
}

int32_t CGameInstance::Update_Priority(_float fDT)
{
	return m_pObjectManager->Update_Priority(fDT);
}

int32_t CGameInstance::Update(_float fDT)
{
	//게임 씬 매니저 업데이트			//주로 디버그용
	m_pPlaceManager->Update(fDT);
	//게임 오브젝트 매니저 업데이트		//진짜 업데이트
	
	return m_pObjectManager->Update(fDT);
}

void CGameInstance::Update_Late( _float fDT)
{
	m_pObjectManager->Update_Late(fDT);
}

void CGameInstance::Begin_Render()
{
	_float4		vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

	if (FAILED(GraphicDevice_Clear_BackBuffer_View(&vClearColor)))
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

	m_pPlaceManager->Render();

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
	RETAIN_POLICY Policy, 
	CPlace* pPlace)
{
	return m_pPlaceManager->Change_Place(Policy, PlaceID, pPlace);
}

inline HRESULT CGameInstance::PlaceManager_Return_Place()
{
	return m_pPlaceManager->Return_Place();
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

HRESULT CGameInstance::PipeLine_Bind_Camera_Pos(CShaderCom* pShader, const _char* pConstantName)
{
	return E_NOTIMPL;
}

#pragma endregion

void CGameInstance::Clear_Engine()
{
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pPlaceManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pPrototype);
	Safe_Release(m_pRenderManager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pGraphicDevice);
	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	CBase::Free();
}
