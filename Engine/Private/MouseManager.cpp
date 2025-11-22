#include "MouseManager.h"
#include "GameInstance.h"

CMouseManager::CMouseManager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMouseManager::Initialize(HWND hWnd, uint32_t ViewportCY)
{
	m_hWnd = hWnd;
	m_iViewportCY = ViewportCY;
	return S_OK;
}

void CMouseManager::Update(_float fDT)
{
	Mouse_Input();
	Calc_Pos();
}

const _float2& CMouseManager::Mouse_Picking()
{
	// DirectX::TriangleTests::Intersects()

	return _float2{ 1.f, 1.f };
}

void CMouseManager::Mouse_Input()
{
	// 버튼 클릭 초기화
	{
		m_isPressStartLBT = FALSE;
		m_isPressStartRBT = FALSE;

		m_isClickLBT = FALSE;
		m_isClickRBT = FALSE;

		m_isPrePressingLBT = m_isPressingLBT;
		m_isPrePressingRBT = m_isPressingRBT;
	}

	
	// 누르고 있는지 확인
	{
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
	}

	// 이제 막 누르기 시작했는지 확인
	{
		if (!m_isPrePressingLBT && m_isPressingLBT)
		{
			m_isPressStartLBT = TRUE;
		}
		if (!m_isPrePressingRBT && m_isPressingRBT)
		{
			m_isPressStartRBT = TRUE;
		}

	}

	// 누르고 있다가 땠는지 확인
	{
		if (m_isPrePressingLBT && !m_isPressingLBT)
		{
			m_isClickLBT = TRUE;
		} 
		if (m_isPrePressingRBT && !m_isPressingRBT)
		{
			m_isClickRBT = TRUE;
		}
	}
}

void CMouseManager::Calc_Pos()
{
	GetCursorPos(&m_vMousePos);
	ScreenToClient(m_hWnd, &m_vMousePos);

	GetCursorPos(&m_vCalcMousePos);
	ScreenToClient(m_hWnd, &m_vCalcMousePos);

	// 각 뷰포트의 LT를 0,0 으로 맞춰줌
	if ((uint32_t)m_vCalcMousePos.y >= m_iViewportCY)
	{
		m_vMousePos.y -= m_iViewportCY;
	}
	// std::cout << "Mouse X : " << m_vCalcMousePos.x << " / Mouse Y : " << m_vCalcMousePos.y << endl;
	// std::cout << m_iViewportCY << endl;
	// std::cout << "Mouse X : " << m_vMousePos.x << " / Mouse Y : " << m_vMousePos.y << endl;
}

CMouseManager* CMouseManager::Create(HWND hWnd, uint32_t ViewportCY)
{
	CMouseManager* pInstance = new CMouseManager;

	if (FAILED(pInstance->Initialize(hWnd, ViewportCY)))
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
