#include "Place_Apart.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

#include "TestMii.h"
#include "Player.h"

CPlace_Apart::CPlace_Apart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Apart::Initialize()
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

int32_t CPlace_Apart::Update(_float fDT)
{
	int a = 0;
	return 0;
}

HRESULT CPlace_Apart::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("아파트다."));
	return S_OK;
}

HRESULT CPlace_Apart::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("아파트다."));
	return S_OK;
}

HRESULT CPlace_Apart::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	// ProtoTag_Actor_Mii
	{
		/*CTestMii::TEST_MII_DESC tMiiDesc = {};
		tMiiDesc.fX = 0.f;
		tMiiDesc.fY = 0.f;
		tMiiDesc.fZ = 0.f;
		tMiiDesc.fCX = 1.f;
		tMiiDesc.fCY = 1.f;
		tMiiDesc.fCZ = 1.f;
		tMiiDesc.fSpeedPerSecond = 10.f;
		tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);
		tMiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(
			static_cast<uint32_t>(PLACEID::APART),
			TEXT("ProtoTag_Actor_Mii"),
			&tMiiDesc);

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
			static_cast<uint32_t>(PLACEID::APART),
			TEXT("LayerTag_Actor"),
			tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}*/

		// ProtoTag_Actor_Player
		{
			CPlayer::ACTOR_DESC tMiiDesc = {};
			tMiiDesc.fX = 0.f;
			tMiiDesc.fY = 0.f;
			tMiiDesc.fZ = 0.f;
			tMiiDesc.fCX = 1.f;
			tMiiDesc.fCY = 1.f;
			tMiiDesc.fCZ = 1.f;
			tMiiDesc.fSpeedPerSecond = 10.f;
			tMiiDesc.fRotationPerSecond = XMConvertToRadians(20.f);
			tMiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;

			tmp = m_pGameInstance->Proto_Get_Object_Clone(
				static_cast<uint32_t>(PLACEID::APART),
				TEXT("ProtoTag_Actor_Player"),
				&tMiiDesc);

			if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
				static_cast<uint32_t>(PLACEID::APART),
				TEXT("LayerTag_Actor"),
				tmp)))
			{
				Safe_Release(tmp);
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT CPlace_Apart::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };

	return S_OK;
}

HRESULT CPlace_Apart::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_Apart::Ready_Camera_Layer()
{
	return S_OK;
}


CPlace_Apart* CPlace_Apart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Apart* pPlaceMain = new CPlace_Apart(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_Apart::Free()
{
	CPlace::Free();
}
