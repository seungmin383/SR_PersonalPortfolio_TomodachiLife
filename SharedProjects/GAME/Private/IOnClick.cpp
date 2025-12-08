#include "IOnClick.h"
#include "GameInstance.h"

IOnClick::IOnClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance(CGameInstance::GetInstance()),
	m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void IOnClick::Free()
{
	CBase::Free();
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}


CNull_Strgy::CNull_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{

}

void CNull_Strgy::OnClick()
{
}

CNull_Strgy* CNull_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CNull_Strgy(pDevice, pContext);
}

void CNull_Strgy::Free()
{
	IOnClick::Free();
}
