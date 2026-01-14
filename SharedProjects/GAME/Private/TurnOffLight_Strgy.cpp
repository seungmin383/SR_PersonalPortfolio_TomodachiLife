#include "TurnOffLight_Strgy.h"	
#include "GameInstance.h"
#include "MiiManager.h"

CTurnOffLight_Strgy::CTurnOffLight_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CTurnOffLight_Strgy::Initialize(_bool isMan)
{
	m_isMan = isMan;
	return S_OK;
}

void CTurnOffLight_Strgy::OnClick()
{
	CMiiManager::GetInstance()->Begin_Edit(m_isMan);
}

CTurnOffLight_Strgy* CTurnOffLight_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool isMan)
{
	CTurnOffLight_Strgy* pInstance = new CTurnOffLight_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(isMan)))
	{
		MSG_BOX("ErrCode : CSetGender_Srtgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTurnOffLight_Strgy::Free()
{
	IOnClick::Free();
}
