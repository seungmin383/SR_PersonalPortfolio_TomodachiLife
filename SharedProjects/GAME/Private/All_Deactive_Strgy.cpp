#include "All_Deactive_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CAll_Deactive_Strgy::CAll_Deactive_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CAll_Deactive_Strgy::Initialize(PLACEID ePlaceID)
{
	m_ePlaceID = ePlaceID;

	return S_OK;
}

void CAll_Deactive_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(m_ePlaceID));
}

CAll_Deactive_Strgy* CAll_Deactive_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PLACEID ePlaceID)
{
	CAll_Deactive_Strgy* pInstance = new CAll_Deactive_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(ePlaceID)))
	{
		MSG_BOX("ErrCode : CAll_Deactive_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAll_Deactive_Strgy::Free()
{
	IOnClick::Free();
}
