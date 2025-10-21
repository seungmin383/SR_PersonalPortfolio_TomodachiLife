#include"Place_Setting.h"
#include "Client_Defines.h"
#include "GameInstance.h"

CPlace_Setting::CPlace_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{

}

HRESULT CPlace_Setting::Initialize()
{
	return S_OK;
}

int32_t CPlace_Setting::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_Setting::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("세팅레벨입니다."));
	return S_OK;
}

HRESULT CPlace_Setting::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("세팅레벨입니다."));
	return S_OK;
}

CPlace_Setting* CPlace_Setting::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext)
{
	CPlace_Setting* pPlaceSetting = new CPlace_Setting(pDevice, pContext);

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