#include "ImgWin.h"
#include "ImgComponent.h"
#include "GameInstance.h"

CImgComponent::CImgComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	Safe_AddRef(m_pGameInstance);
}

HRESULT	CImgComponent::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return S_OK;
}

void CImgComponent::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

}
