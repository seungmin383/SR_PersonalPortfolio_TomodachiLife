#include "SetLovePlace_Strgy.h"	
#include "GameInstance.h"

#include "ApartManager.h"

CSetLovePlace_Strgy::CSetLovePlace_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetLovePlace_Strgy::Initialize(uint32_t i)
{
	m_iPlace = i;
	return S_OK;
}

void CSetLovePlace_Strgy::OnClick()
{
	CApartManager::GetInstance()->Set_iLovePlace(m_iPlace);
}

CSetLovePlace_Strgy* CSetLovePlace_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t i)
{
	CSetLovePlace_Strgy* pInstance = new CSetLovePlace_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(i)))
	{
		MSG_BOX("ErrCode : CSetLovePlace_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetLovePlace_Strgy::Free()
{
	IOnClick::Free();
}
