#include "SetEditInfo_isMan_Strgy.h"	
#include "GameInstance.h"
#include"MiiManager.h"


CSetEditInfo_isMan_Strgy::CSetEditInfo_isMan_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetEditInfo_isMan_Strgy::Initialize(_bool is)
{
	m_isMan = is;
	return S_OK;
}

void CSetEditInfo_isMan_Strgy::OnClick()
{

	if (CMiiManager::GetInstance()->Get_Edit_Profile().isMan != m_isMan)
	{
		CMiiManager::GetInstance()->Get_Edit_Profile().isMan = m_isMan;

		if (CMiiManager::GetInstance()->Get_Edit_Profile().isMan)
		{
			CMiiManager::GetInstance()->Get_Edit_HeadInfo().iIndex = 0;
			CMiiManager::GetInstance()->Get_Edit_HairInfo().iIndex = 1;
			CMiiManager::GetInstance()->Get_Edit_EyebrowInfo().iIndex = 78;
			CMiiManager::GetInstance()->Get_Edit_EyesInfo().iIndex = 0;
			CMiiManager::GetInstance()->Get_Edit_NoseInfo().iIndex = 60;
			CMiiManager::GetInstance()->Get_Edit_MouthInfo().iIndex = 101;
		}
		else
		{
			CMiiManager::GetInstance()->Get_Edit_HeadInfo().iIndex = 0;
			CMiiManager::GetInstance()->Get_Edit_HairInfo().iIndex = 0;
			CMiiManager::GetInstance()->Get_Edit_EyebrowInfo().iIndex = 79;
			CMiiManager::GetInstance()->Get_Edit_EyesInfo().iIndex = 1;
			CMiiManager::GetInstance()->Get_Edit_NoseInfo().iIndex = 60;
			CMiiManager::GetInstance()->Get_Edit_MouthInfo().iIndex = 101;
		}
	}

}

CSetEditInfo_isMan_Strgy* CSetEditInfo_isMan_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool is)
{
	CSetEditInfo_isMan_Strgy* pInstance = new CSetEditInfo_isMan_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(is)))
	{
		MSG_BOX("ErrCode : CSetEditInfo_isMan_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetEditInfo_isMan_Strgy::Free()
{
	IOnClick::Free();
}
