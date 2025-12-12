#include "BeginMiiEdit.h"	
#include "GameInstance.h"
#include "MiiManager.h"

CBeginMiiEdit_Strgy::CBeginMiiEdit_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CBeginMiiEdit_Strgy::Initialize(_bool isMan)
{
	m_isMan = isMan;
	return S_OK;
}

void CBeginMiiEdit_Strgy::OnClick()
{
	CMiiManager::GetInstance()->Begin_Edit(m_isMan);
}

CBeginMiiEdit_Strgy* CBeginMiiEdit_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool isMan)
{
	CBeginMiiEdit_Strgy* pInstance = new CBeginMiiEdit_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(isMan)))
	{
		MSG_BOX("ErrCode : CSetGender_Srtgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBeginMiiEdit_Strgy::Free()
{
	IOnClick::Free();
}
