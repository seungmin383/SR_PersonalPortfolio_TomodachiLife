#include "SetLove_Strgy.h"	
#include "GameInstance.h"

#include "ApartManager.h"

CSetLove_Strgy::CSetLove_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetLove_Strgy::Initialize(_bool is)
{
	m_isLove = is;
	return S_OK;
}

void CSetLove_Strgy::OnClick()
{
	CApartManager::GetInstance()->Set_isLove(m_isLove);
}

CSetLove_Strgy* CSetLove_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool is)
{
	CSetLove_Strgy* pInstance = new CSetLove_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(is)))
	{
		MSG_BOX("ErrCode : CSetLove_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetLove_Strgy::Free()
{
	IOnClick::Free();
}
