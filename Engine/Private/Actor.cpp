#include "Actor.h"

CActor::CActor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CObject(),
	m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CActor::CActor(const CActor& rhs)
	:CObject(rhs),
	m_pDevice(rhs.m_pDevice), m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


void CActor::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CObject::Free();
}
