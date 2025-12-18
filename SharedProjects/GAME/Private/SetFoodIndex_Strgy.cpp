#include "SetFoodIndex_Strgy.h"	
#include "GameInstance.h"

#include "ItemManager.h"

CSetFoodIndex_Strgy::CSetFoodIndex_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSetFoodIndex_Strgy::Initialize(uint32_t iIndex)
{
	m_iIndex = iIndex;
	return S_OK;
}

void CSetFoodIndex_Strgy::OnClick()
{
	CItemManager::GetInstance()->Set_ItemIndex(ITEM_TYPE::FOOD, m_iIndex);
}

CSetFoodIndex_Strgy* CSetFoodIndex_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t iIndex)
{
	CSetFoodIndex_Strgy* pInstance = new CSetFoodIndex_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(iIndex)))
	{
		MSG_BOX("ErrCode : CSetFoodIndex_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSetFoodIndex_Strgy::Free()
{
	IOnClick::Free();
}
