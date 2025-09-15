#include "MainApp.h"
#include "GameInstance.h"

USING(Client)

CMainApp::CMainApp()
{

}

HRESULT CMainApp::Initialize()
{
	if(FAILED(CGameInstance::GetInstance()->Initialize()))
		return E_FAIL;

	return S_OK;
}

int32_t CMainApp::Update(_float fTimeDelta)
{
	return 0;
}

void CMainApp::Render()
{
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
}
