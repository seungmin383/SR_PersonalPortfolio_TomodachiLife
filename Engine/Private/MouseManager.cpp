#include "MouseManager.h"
#include "GameInstance.h"

CMouseManager::CMouseManager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMouseManager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	return S_OK;
}

void CMouseManager::Update(_float fDT)
{
	Mouse_Input();
	GetCursorPos(&m_vMousePos);
	ScreenToClient(m_hWnd, &m_vMousePos);
}

void CMouseManager::Mouse_Input()
{
	m_isClickLBT = FALSE;
	m_isClickRBT = FALSE;

	m_isPrePressingLBT = m_isPressingLBT;
	m_isPrePressingRBT = m_isPressingRBT;

	if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::LB) & 0x80)
	{
		m_isPressingLBT = TRUE;
	}
	else
	{
		m_isPressingLBT = FALSE;
	}

	if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::RB) & 0x80)
	{
		m_isPressingRBT = TRUE;
	}
	else
	{
		m_isPressingRBT = FALSE;
	}

	if (m_isPrePressingLBT && !m_isPressingLBT)
	{
		m_isClickLBT = TRUE;
	}

	if (m_isPrePressingRBT && !m_isPressingRBT)
	{
		m_isClickRBT = TRUE;
	}
}



CMouseManager* CMouseManager::Create(HWND hWnd)
{
	CMouseManager* pInstance = new CMouseManager;

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("ErrCode : MouseManager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMouseManager::Free()
{
	Safe_Release(m_pGameInstance);
	CBase::Free();
}
