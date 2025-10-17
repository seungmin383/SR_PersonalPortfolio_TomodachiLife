#include "Place_Main.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CPlace_Main::CPlace_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Main::Initialize()
{
	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("ProtoTag_UI_Food"),
		nullptr);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(Client::PLACE_ID::MAIN),
		TEXT("LayerTag_UI"),
		tmp)))
	{
		Safe_Release(tmp);
		return E_FAIL;
	}

	return S_OK;
}

int32_t CPlace_Main::Update(_float fDT)
{
	if (GetAsyncKeyState('A') & 0x8000)
		m_pGameInstance->PlaceManager_Change_Place(
			static_cast<uint32_t>(PLACE_ID::SETTING),
			Engine::RETAIN_POLICY::CLEAR_ALL,
			CPlace_Loading::Create(
				m_pDevice,
				m_pContext,
				PLACE_ID::SETTING,
				RETAIN_POLICY::CLEAR_LOADING));

	return 0;
}

HRESULT CPlace_Main::Render()
{
	SetWindowText(g_hWnd, TEXT("메인레벨입니다."));
	return S_OK;
}


CPlace_Main* CPlace_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_Main* pPlaceMain = new CPlace_Main(pDevice, pContext);

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
