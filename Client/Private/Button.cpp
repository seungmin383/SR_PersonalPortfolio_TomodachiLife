#include "PlaceMoveButton.h"
#include "Button.h"
#include "GameInstance.h"

CButton::CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CButton::CButton(const CButton& rhs)
	:CUI(rhs)
{
}

HRESULT CButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CButton::Initialize_Clone(void* pArg)
{
	BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(pArg);

	m_eViewport = pDesc->eViewport;

	CUI::Initialize_Clone(pArg);

	m_pContext->RSGetViewports(&m_iNumViewport, &m_CurViewport);

	return S_OK;
}

int32_t CButton::Update_Priority(_float fDT)
{
	POINT tmp = m_pGameInstance->MouseManager_Get_MousePos();

	switch (m_eViewport)
	{
	case Engine::VIEWPORT_TYPE::TOP:
		Compare_Position(tmp);
		break;
	case Engine::VIEWPORT_TYPE::BOTTOM:
		tmp.y -= 480;
		Compare_Position(tmp);
		break;

	case Engine::VIEWPORT_TYPE::END:
	default:
		return 1;
	}

	return 0;
}

int32_t CButton::Update(_float fDT)
{
	return 0;
}

void CButton::Update_Late(_float fDT)
{
}

HRESULT CButton::Render()
{
	return S_OK;
}

_bool CButton::Compare_Position(const POINT& vMousePos)
{
	_float fLeftX = m_fX - (m_fCX / 2.f);
	_float fRightX = m_fX + (m_fCX / 2.f);

	_float fTopY = m_fY - (m_fCY / 2.f);
	_float fBottomY = m_fY + (m_fCY / 2.f);

	_bool isX = { FALSE };
	_bool isY = { FALSE };

	if (vMousePos.x >= fLeftX && vMousePos.x <= fRightX)
		isX = TRUE;

	if (vMousePos.y >= fTopY && vMousePos.y <= fBottomY)
		isY = TRUE;

	if (isX && isY)
	{
		m_isClick = TRUE;
		m_isEverClicked = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

void CButton::Free()
{
	CUI::Free();
}
