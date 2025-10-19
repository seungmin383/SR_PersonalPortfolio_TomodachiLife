#include"Place_Setting.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "MainCamera.h"

CPlace_Setting::CPlace_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{

}

HRESULT CPlace_Setting::Initialize()
{
	CObject* tmp = { nullptr };

// Island // ========================================================
	tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("ProtoTag_Actor_Island"),
		nullptr);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("LayerTag_Terrain"),
		tmp)))
		return E_FAIL;

	CMainCamera::MAINCAMERA_DESC tDesc = {};

	tDesc.fRotationPerSecond = XMConvertToRadians(30.f);
	tDesc.fSpeedPerSecond = 100.f;

	tDesc.m_fAspect = static_cast<float>(WINCX) / static_cast<float>(WINCY);
	tDesc.m_fFov = XMConvertToRadians(90.0f);

	tDesc.m_fNear = 0.1f;
	tDesc.m_fFar = 1000.f;

	tDesc.vEye = _float3(0.f, 30.f, -20.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);

	tDesc.fSensor = 10.f;

	tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::SETTING),
		TEXT("ProtoTag_Actor_MainCamera"),
		&tDesc);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(PLACE_ID::SETTING),
		TEXT("LayerTag_Camera"),
		tmp)))
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_Setting::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_Setting::Render()
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