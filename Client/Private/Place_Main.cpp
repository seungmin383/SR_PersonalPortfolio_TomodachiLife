#include "Place_Main.h"
#include "Client_Defines.h"

CPlace_Main::CPlace_Main(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CPlace(_pDevice, _pContext)
{
}

HRESULT CPlace_Main::Initialize()
{
	return S_OK;
}

int32_t CPlace_Main::Update(_float fTimeDelta)
{
	return 0;
}

HRESULT CPlace_Main::Render()
{
	return S_OK;
}

CPlace_Main* CPlace_Main::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlace_Main* pPlaceMain = new CPlace_Main(_pDevice, _pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Main Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Main::Free()
{
}
