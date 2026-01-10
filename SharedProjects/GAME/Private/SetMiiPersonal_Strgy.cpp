#include "SetMiiPersonal_Strgy.h"	
#include "GameInstance.h"
#include"MiiManager.h"


CSetMiiPersonal_Strgy::CSetMiiPersonal_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetMiiPersonal_Strgy::Initialize(int32_t i)
{
	m_iPersonal = i;

	return S_OK;
}

void CSetMiiPersonal_Strgy::OnClick()
{
	CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal += m_iPersonal;
}

CSetMiiPersonal_Strgy* CSetMiiPersonal_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int32_t i)
{
	CSetMiiPersonal_Strgy* pInstance = new CSetMiiPersonal_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(i)))
	{
		MSG_BOX("ErrCode : CSetMiiPersonal_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetMiiPersonal_Strgy::Free()
{
	IOnClick::Free();
}
