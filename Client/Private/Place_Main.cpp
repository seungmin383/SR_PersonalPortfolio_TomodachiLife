#include "Place_Main.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Place_Loading.h"

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
	if (GetAsyncKeyState('A'))
		m_pGameInstance->PlaceManager_Go_Place(static_cast<int32_t>(PLACE_ID::LOADING), CPlace_Loading::Create(m_pDevice, m_pContext, (PLACE_ID::SETTING)));

	return 0;
}

HRESULT CPlace_Main::Render()
{
	SetWindowText(g_hWnd, TEXT("메인레벨입니다."));
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
	CPlace::Free();
}
