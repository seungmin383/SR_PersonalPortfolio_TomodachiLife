#include "SetisBeFriend_Strgy.h"	
#include "GameInstance.h"

#include "ApartManager.h"

CSetisBeFriend_Strgy::CSetisBeFriend_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetisBeFriend_Strgy::Initialize(_bool is)
{
	m_isBeFriend = is;
	return S_OK;
}

void CSetisBeFriend_Strgy::OnClick()
{
	CApartManager::GetInstance()->Set_isBeFriend(m_isBeFriend);
}

CSetisBeFriend_Strgy* CSetisBeFriend_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool is)
{
	CSetisBeFriend_Strgy* pInstance = new CSetisBeFriend_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(is)))
	{
		MSG_BOX("ErrCode : CSetisBeFriend_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetisBeFriend_Strgy::Free()
{
	IOnClick::Free();
}
