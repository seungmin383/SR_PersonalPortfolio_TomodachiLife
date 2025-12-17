#include "NoneClick.h"	
#include "GameInstance.h"

CNoneClick::CNoneClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CNoneClick::Initialize()
{
	return S_OK;
}

void CNoneClick::OnClick()
{
}

CNoneClick* CNoneClick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNoneClick* pInstance = new CNoneClick(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CSetGender_Srtgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CNoneClick::Free()
{
	IOnClick::Free();
}
