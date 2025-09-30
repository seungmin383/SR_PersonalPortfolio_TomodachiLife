#include "Component.h"

CComponent::CComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CObject(_pDevice, _pContext),
	m_isClone(false)
{
}

CComponent::CComponent(const CComponent& rhs)
	:CObject(rhs),
	m_isClone(true)
{
}

HRESULT CComponent::Initialize_Origin()
{
	return S_OK;
}

HRESULT CComponent::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CComponent::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CComponent::Update(const _float _fDT)
{
	return 0;
}

void CComponent::Update_Late(const _float _fDT)
{
}

HRESULT CComponent::Render()
{
	return S_OK;
}

void CComponent::Free()
{
	CObject::Free();
}

