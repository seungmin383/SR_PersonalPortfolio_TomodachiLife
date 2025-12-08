#include "Deactive_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CDeactive_Strgy::CDeactive_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CDeactive_Strgy::Initialize(PLACEID ePlaceID, const _wstring& strLayerTag)
{
	m_ePlaceID = ePlaceID;
	m_strLayerTag = strLayerTag;

	return S_OK;
}

void CDeactive_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(m_ePlaceID), m_strLayerTag);
}

CDeactive_Strgy* CDeactive_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PLACEID ePlaceID, const _wstring& strLayerTag)
{
	CDeactive_Strgy* pInstance = new CDeactive_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(ePlaceID, strLayerTag)))
	{
		MSG_BOX("ErrCode : CDeactive_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CDeactive_Strgy::Free()
{
	IOnClick::Free();
}
