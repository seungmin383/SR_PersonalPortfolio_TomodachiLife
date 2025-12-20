#include "BuyFood_Strgy.h"	
#include "GameInstance.h"

#include "ItemManager.h"
#include "ApartManager.h"

CBuyFood_Strgy::CBuyFood_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CBuyFood_Strgy::Initialize( )
{
	return S_OK;
}

void CBuyFood_Strgy::OnClick()
{
	 CApartManager::GetInstance()->Get_ApartInfo_Money();

	int32_t iPrice = CItemManager::GetInstance()->Get_CurFoodInfo().iPrice;
	int32_t iMoney = CApartManager::GetInstance()->Get_ApartInfo_Money();

	if (iMoney - iPrice < 0)
	{
		MSG_BOX("돈이 부족");
		return;
	}

	/* 구입되었다는 가정 */
	CApartManager::GetInstance()->Minus_ApartInfo_Money(iPrice);
	CApartManager::GetInstance()->Add_FoodInventory(CItemManager::GetInstance()->Get_CurFoodItemIndex());
}

CBuyFood_Strgy* CBuyFood_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext  )
{
	CBuyFood_Strgy* pInstance = new CBuyFood_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CBuyFood_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBuyFood_Strgy::Free()
{
	IOnClick::Free();
}
