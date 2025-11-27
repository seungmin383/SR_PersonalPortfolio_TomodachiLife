#include "Object.h"
#include "GameInstance.h"

CObject::CObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CObject::CObject(const CObject& rhs)
	:m_pGameInstance(rhs.m_pGameInstance),
	m_pDevice(rhs.m_pDevice), m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CObject::Initialize_Origin()
{
	return S_OK;
}

HRESULT CObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CObject::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CObject::Update(_float fDT)
{
	return 0;
}

void CObject::Update_Late(_float fDT)
{
	return;
}

HRESULT CObject::Render()
{
	ID3D11ShaderResourceView* NullSRV[16] = {};
	m_pContext->PSSetShaderResources(0, 16, NullSRV);

	return S_OK;
}

void CObject::Free()
{
	CBase::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

}
