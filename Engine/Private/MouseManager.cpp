#include "MouseManager.h"

CMouseManager::CMouseManager()
{
}

HRESULT CMouseManager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	return S_OK;
}

void CMouseManager::Update(_float fDT)
{
	GetCursorPos(&m_vMousePos);
	ScreenToClient(m_hWnd, &m_vMousePos);
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
	CBase::Free();
}
