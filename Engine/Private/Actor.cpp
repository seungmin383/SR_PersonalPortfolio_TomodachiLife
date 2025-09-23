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

HRESULT CActor::Initialize_Origin()
{
	return S_OK;
}

HRESULT CActor::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CActor::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CActor::Update(const _float _fDT)
{
	return 0;
}

void CActor::Update_Late(const _float _fDT)
{
	return;
}

HRESULT CActor::Render()
{
	return S_OK;
}

void CActor::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CObject::Free();
}
