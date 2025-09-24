#include "MainApp.h"
#include "GameInstance.h"
#include "Place_Loading.h"

USING(Client)

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	if(FAILED(m_pGameInstance->Initialize()))
		return E_FAIL;

	m_pGameInstance->GraphicDevice_Initialize(g_hWnd, WINCX, WINCY, &m_pDevice, &m_pContext);

	CPlace* tmp = CPlace_Loading::Create(m_pDevice, m_pContext, PLACE_ID::MAIN);
	int a = 0;

	if (FAILED(m_pGameInstance->PlaceManager_Go_Place(static_cast<int32_t>(PLACE_ID::LOADING), tmp)))
	{
		MSG_BOX("ErrCode : Go_Place Failed");
		return E_FAIL;
	}

	return S_OK;
}

int32_t CMainApp::Update_Priority(_float _fDT)
{
	return m_pGameInstance->Update_Priority(_fDT);
}

int32_t CMainApp::Update(_float _fDT)
{
	return m_pGameInstance->Update(_fDT);
}

void CMainApp::Update_Late(_float _fDT)
{
	m_pGameInstance->Update_Late(_fDT);
}

void CMainApp::Render()
{
	m_pGameInstance->Begin_Render();

	m_pGameInstance->Render();

	m_pGameInstance->End_Render();
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (nullptr == pMainApp)
	{
		MSG_BOX("ErrCode : MainApp Create Failed");
		Safe_Release(pMainApp);
		return nullptr;
	}

	if (FAILED(pMainApp->Initialize()))
	{
		MSG_BOX("ErrCode : MainApp Initialize Failed");
		Safe_Release(pMainApp);
		return nullptr;
	}

	return pMainApp;
}

void CMainApp::Free()
{
	CBase::Free();


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Clear_Engine();
	Safe_Release(m_pGameInstance);
}
