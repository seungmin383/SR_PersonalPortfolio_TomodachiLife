#include "SetLoveMethod_Strgy.h"	
#include "GameInstance.h"

#include "ApartManager.h"

CSetLoveMethod_Strgy::CSetLoveMethod_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetLoveMethod_Strgy::Initialize(uint32_t i)
{
	m_iMethod = i;
	return S_OK;
}

void CSetLoveMethod_Strgy::OnClick()
{
	CApartManager::GetInstance()->Set_iLoveMethod(m_iMethod);
}

CSetLoveMethod_Strgy* CSetLoveMethod_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t i)
{
	CSetLoveMethod_Strgy* pInstance = new CSetLoveMethod_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(i)))
	{
		MSG_BOX("ErrCode : CSetLoveMethod_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetLoveMethod_Strgy::Free()
{
	IOnClick::Free();
}
