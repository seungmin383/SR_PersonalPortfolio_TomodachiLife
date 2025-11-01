#include "Place_Island.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

CPlace_Island::CPlace_Island(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Island::Initialize()
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

int32_t CPlace_Island::Update(_float fDT)
{

	return 0;
}

HRESULT CPlace_Island::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("¼¶ÀÌ´Ù."));
	return S_OK;
}

HRESULT CPlace_Island::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("¼¶ÀÌ´Ù."));
	return S_OK;
}

HRESULT CPlace_Island::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	CActor::ACTOR_DESC tMiiDesc = {};
	tMiiDesc.fX = 0.f;
	tMiiDesc.fY = 0.f;
	tMiiDesc.fZ = 0.f;
	tMiiDesc.fCX = 1.f;
	tMiiDesc.fCY = 1.f;
	tMiiDesc.fCZ = 1.f;
	tMiiDesc.fSpeedPerSecond = 0.f;
	tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);


	tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(PLACE_ID::ISLAND),
		TEXT("ProtoTag_Actor_Island"),
		&tMiiDesc);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(PLACE_ID::ISLAND),
		TEXT("LayerTag_Actor"),
		tmp)))
	{
		Safe_Release(tmp);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_Island::Ready_UI_Layer()
{
	return S_OK;
}

HRESULT CPlace_Island::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Island::Ready_Camera_Layer()
{




	return S_OK;
}


CPlace_Island* CPlace_Island::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Island* pPlaceMain = new CPlace_Island(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Island Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Island::Free()
{
	CPlace::Free();
}
