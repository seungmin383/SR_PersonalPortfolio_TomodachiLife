#include "ISelect_Strgy.h"

ISelect_Strgy::ISelect_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void ISelect_Strgy::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
