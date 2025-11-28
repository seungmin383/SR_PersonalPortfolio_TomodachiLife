#include "SaveChangePlace_Strategy.h"
#include "MiiManager.h"

CSaveChangePlace_Strategy::CSaveChangePlace_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSaveChangePlace_Strategy::Initialize()
{
	return S_OK;
}

void CSaveChangePlace_Strategy::OnClick()
{
	ofstream Out(TEXT(""));
}

CSaveChangePlace_Strategy* CSaveChangePlace_Strategy::Create(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSaveChangePlace_Strategy* pInstance = new CSaveChangePlace_Strategy(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CSaveChangePlace_Strategy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSaveChangePlace_Strategy::Free()
{
	IOnClick::Free();
}

