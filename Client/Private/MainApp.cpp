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

	CGameInstance::GetInstance()->GraphicDevice_Initialize(g_hWnd, WINCX, WINCY, &m_pDevice, &m_pContext);

	return S_OK;
}

int32_t CMainApp::Update(_float fTimeDelta)
{
	return 0;
}

void CMainApp::Render()
{
	const _float4 fColor = { 0.f, 0.f, 255.f, 0.f };
	CGameInstance::GetInstance()->GraphicDevice_Clear_BackBuffer_View(&fColor);
	CGameInstance::GetInstance()->GraphicDevice_Present();
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
	CGameInstance::DestroyInstance();

	CBase::Free();
}
