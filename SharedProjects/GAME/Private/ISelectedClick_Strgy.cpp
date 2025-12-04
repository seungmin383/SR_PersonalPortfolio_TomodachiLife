#include "ISelectedClick_Strgy.h"

ISelectedClick_Strgy::ISelectedClick_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void ISelectedClick_Strgy::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
