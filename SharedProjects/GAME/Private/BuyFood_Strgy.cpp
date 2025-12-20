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
	CApartManager::GetInstance()->Minus_ApartInfo_Money(CItemManager::GetInstance()->Get_CurFoodInfo().iPrice);
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
