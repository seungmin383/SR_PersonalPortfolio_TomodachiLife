#include "PlaceMoveButton.h"

CPlaceMoveButton::CPlaceMoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CPlaceMoveButton::CPlaceMoveButton(const CPlaceMoveButton& rhs)
	:CButton(rhs)
{
}

HRESULT CPlaceMoveButton::Initialize_Origin()
{
	return E_NOTIMPL;
}

HRESULT CPlaceMoveButton::Initialize_Clone(void* pArg)
{
	CButton::Initialize_Clone(pArg);
	return S_OK;
}

int32_t CPlaceMoveButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);	// 마우스랑 확인
	return 0;
}

int32_t CPlaceMoveButton::Update(_float fDT)
{
	return 0;
}

void CPlaceMoveButton::Update_Late(_float fDT)
{
}

HRESULT CPlaceMoveButton::Render()
{
	return E_NOTIMPL;
}

void CPlaceMoveButton::Free()
{
	CButton::Free();
}
