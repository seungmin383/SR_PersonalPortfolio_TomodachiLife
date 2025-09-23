#include"Place_Setting.h"
#include "Client_Defines.h"

CPlace_Setting::CPlace_Setting(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CPlace(_pDevice, _pContext)
{

}

HRESULT CPlace_Setting::Initialize()
{
	return S_OK;
}

int32_t CPlace_Setting::Update(_float fTimeDelta)
{
	return 0;
}

HRESULT CPlace_Setting::Render()
{
	return S_OK;
}

CPlace_Setting* CPlace_Setting::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlace_Setting* pPlaceSetting = new CPlace_Setting(_pDevice, _pContext);

	if (FAILED(pPlaceSetting->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Setting Create Failed");
		Safe_Release(pPlaceSetting);
	}
	return pPlaceSetting;
}

void CPlace_Setting::Free()
{
	CPlace::Free();
}