#include "ChangePlace_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CChangePlace_Strgy::CChangePlace_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IClick_Strgy(pDevice, pContext)
{
}

HRESULT CChangePlace_Strgy::Initialize(void* pArg)
{
	CAMERAZOOMIN_DESC* pDesc = static_cast<CAMERAZOOMIN_DESC*>(pArg);

	return S_OK;
}

int32_t CChangePlace_Strgy::OnClicked()
{
	m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(m_eNextPlaceID),
		CPlace_Loading::Create(m_pDevice, m_pContext, m_eNextPlaceID));
	return UPDATE_CLEAR;
}

CChangePlace_Strgy* CChangePlace_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CChangePlace_Strgy* pInstance = new CChangePlace_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("ErrCode : CChangePlace_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CChangePlace_Strgy::Free()
{
	__super::Free();
}

