#include "SetUIisEnable_Strgy.h"	
#include "GameInstance.h"

#include "ApartManager.h"

CSetUIisEnable_Strgy::CSetUIisEnable_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetUIisEnable_Strgy::Initialize(_bool is)
{
	m_isEnable = is;

	return S_OK;
}

void CSetUIisEnable_Strgy::OnClick()
{
	m_pGameInstance->UIManager_Set_isEnable(m_isEnable);
}

CSetUIisEnable_Strgy* CSetUIisEnable_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool is)
{
	CSetUIisEnable_Strgy* pInstance = new CSetUIisEnable_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(is)))
	{
		MSG_BOX("ErrCode : CSetUIisEnable_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetUIisEnable_Strgy::Free()
{
	IOnClick::Free();
}
