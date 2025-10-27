#include "Place.h"
#include "GameInstance.h"
#include "Object.h"

CPlace::CPlace(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CPlace::Initialize()
{
	return S_OK;
}

int32_t CPlace::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace::Render_Priority()
{
	return S_OK;
}

HRESULT CPlace::Render_Late()
{
	return S_OK;
}

void CPlace::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CBase::Free();
}
