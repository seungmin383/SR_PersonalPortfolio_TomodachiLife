#include "ImgWin.h"

CImgWin::CImgWin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT	CImgWin::Initialize()
{
	return S_OK;
}

void CImgWin::Update_Priority(_float fDT)
{

}

void CImgWin::Update(_float fDT)
{

}

void CImgWin::Update_Late(_float fDT)
{

}

void CImgWin::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
