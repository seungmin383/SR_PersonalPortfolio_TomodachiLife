#include "Place_MiiEditor.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

CPlace_MiiEditor::CPlace_MiiEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_MiiEditor::Initialize()
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

int32_t CPlace_MiiEditor::Update(_float fDT)
{

	return 0;
}

HRESULT CPlace_MiiEditor::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("mii Editor"));
	return S_OK;
}

HRESULT CPlace_MiiEditor::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("mii Editor"));
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };

	
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_UI_Layer()
{
	
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_Camera_Layer()
{




	return S_OK;
}


CPlace_MiiEditor* CPlace_MiiEditor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_MiiEditor* pInstance = new CPlace_MiiEditor(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : Place_MiiEditor Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlace_MiiEditor::Free()
{
	CPlace::Free();
}
