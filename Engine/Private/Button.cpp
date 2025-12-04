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
	CUI::Initialize_Clone(pArg);	// 트랜스폼 만들고

	_float fLeftX	= m_fX - (m_fCX / 2.f);
	_float fRightX	= m_fX + (m_fCX / 2.f);

	_float fTopY	= m_fY - (m_fCY / 2.f);
	_float fBottomY = m_fY + (m_fCY / 2.f);

	m_fLeftTop.x = fLeftX;
	m_fLeftTop.y = fTopY;
	m_fRIghtBottom.x = fRightX;
	m_fRIghtBottom.y = fBottomY;

	return S_OK;
}

int32_t CButton::Update_Priority(_float fDT)
{
	Compare_Position(m_pGameInstance->MouseManager_Get_MousePos());
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
	__super::Render();
	return S_OK;
}

void CButton::Compare_Position(const POINT& vMousePos)
{
	// 초기화 후 다시 계산
	{
		m_isClicked = FALSE;
		m_isOverlappedMouse = FALSE;
	}

	_bool isOverlapMouseX = { FALSE };
	_bool isOverlapMouseY = { FALSE };

	if (vMousePos.x >= m_fLeftTop.x && vMousePos.x <= m_fRIghtBottom.x)
		isOverlapMouseX = TRUE;

	if (vMousePos.y >= m_fLeftTop.y && vMousePos.y <= m_fRIghtBottom.y)
		isOverlapMouseY = TRUE;

	if (isOverlapMouseX && isOverlapMouseY)
		m_isOverlappedMouse = TRUE;
	else
		m_isOverlappedMouse = FALSE;
}

int32_t CButton::On_Click()
{
	return 0;
}
int32_t CButton::On_Select()
{
	return 0;
}

int32_t CButton::On_SelectedClick()
{
	return 0;
}

void CButton::Free()
{
	CUI::Free();
}
