#include"Place_Setting.h"
#include "GameInstance.h"
#include "WorldCamera.h"
#include "UICamera.h"

CPlace_Setting::CPlace_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{

}

HRESULT CPlace_Setting::Initialize()
{
	if (FAILED(Ready_Setting_Actor_Layer()))
		return E_FAIL;
	if(FAILED(Ready_Setting_UI_Layer()))
		return E_FAIL;
	if(FAILED(Ready_Setting_Environment_Layer()))
		return E_FAIL;
	if(FAILED(Ready_Setting_Camera_Layer()))
		return E_FAIL;

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

HRESULT CPlace_Setting::Ready_Setting_Actor_Layer()
{
	//CActor::ACTOR_DESC tDesc = {};
	//tDesc.fX = 0.f;
	//tDesc.fY = 0.f;
	//tDesc.fZ = 0.f;
	//tDesc.fCX = 1.f;
	//tDesc.fCY = 1.f;
	//tDesc.fCZ = 1.f;
	//tDesc.fSpeedPerSecond = 0.f;
	//tDesc.fRotationPerSecond = XMConvertToRadians(20.f);

	//CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
	//	static_cast<uint32_t>(PLACEID::SETTING),
	//	TEXT("ProtoTag_Actor_Island"),
	//	&tDesc);

	//if (nullptr == tmp)
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
	//	static_cast<uint32_t>(PLACEID::SETTING),
	//	TEXT("LayerTag_Actor"),
	//	tmp)))
	//{
	//	Safe_Release(tmp);
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT CPlace_Setting::Ready_Setting_UI_Layer()
{
	//CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
	//	static_cast<uint32_t>(PLACEID::SETTING),
	//	TEXT("ProtoTag_UI_Food"),
	//	nullptr);

	//if (nullptr == tmp)
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
	//	static_cast<uint32_t>(PLACEID::SETTING),
	//	TEXT("LayerTag_UI"),
	//	tmp)))
	//{
	//	Safe_Release(tmp);
	//	return E_FAIL;
	//}


	return S_OK;
}

HRESULT CPlace_Setting::Ready_Setting_Environment_Layer()
{
	return E_NOTIMPL;
}

HRESULT CPlace_Setting::Ready_Setting_Camera_Layer()
{
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