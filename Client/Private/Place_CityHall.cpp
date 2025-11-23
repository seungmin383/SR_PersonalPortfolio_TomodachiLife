#include "Place_CityHall.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

CPlace_CityHall::CPlace_CityHall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_CityHall::Initialize()
{
	if (FAILED(Ready_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Camera_Layer()))
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_CityHall::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_CityHall::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("ÁÖ¹Î¼¾ÅÍ´Ù."));
	return S_OK;
}

HRESULT CPlace_CityHall::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("ÁÖ¹Î¼¾ÅÍ´Ù."));
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	return S_OK;
}

HRESULT CPlace_CityHall::Ready_UI_Layer()
{
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_CityHall::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	return S_OK;
}


CPlace_CityHall* CPlace_CityHall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_CityHall* pPlaceMain = new CPlace_CityHall(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_CityHall::Free()
{
	CPlace::Free();
}
