#include "Place_MiiEditor.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"
#include "MiiEditorBG.h"

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

	/* Mii 에디터 배경 모델 */
	// ProtoTag_Component_EditorBG
	{
		CMiiEditorBG::MIIEDITORBG_DESC tMiiEditorDesc = {};
		tMiiEditorDesc.fCX = 1.f;
		tMiiEditorDesc.fCY = 1.f;
		tMiiEditorDesc.fCZ = 1.f;
		tMiiEditorDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		tMiiEditorDesc.vPos = XMFLOAT4{ 0.f, 0.f, -10.f, 1.f };

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("ProtoTag_Component_EditorBG"), &tMiiEditorDesc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_Actor"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}
	
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
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
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
