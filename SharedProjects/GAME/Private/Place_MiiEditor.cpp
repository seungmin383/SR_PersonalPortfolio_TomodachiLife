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
	if (FAILED(Ready_MiiEditor_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_MiiEditor_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_MiiEditor_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_MiiEditor_Camera_Layer()))
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_MiiEditor::Update(_float fDT)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_pGameInstance->PlaceManager_Change_Place(
			static_cast<uint32_t>(PLACE_ID::SETTING),
			CPlace_Loading::Create(
				m_pDevice,
				m_pContext,
				PLACE_ID::SETTING));

	return 0;
}

HRESULT CPlace_MiiEditor::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("Mii 에디터 레벨입니다."));
	return S_OK;
}

HRESULT CPlace_MiiEditor::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("Mii 에디터 레벨입니다."));
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_MiiEditor_Actor_Layer()
{
	CActor::ACTOR_DESC tDesc = {};
	tDesc.fX = 0.f;
	tDesc.fY = 0.f;
	tDesc.fZ = 0.f;
	tDesc.fCX = 1.f;
	tDesc.fCY = 1.f;
	tDesc.fCZ = 1.f;
	tDesc.fSpeedPerSecond = 0.f;
	tDesc.fRotationPerSecond = XMConvertToRadians(20.f);

	CObject* tmp = m_pGameInstance->Proto_Get_Object_Clone(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("ProtoTag_Actor_Island"),
		&tDesc);

	if (nullptr == tmp)
		return E_FAIL;

	if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(
		static_cast<uint32_t>(PLACE_ID::MAIN),
		TEXT("LayerTag_Actor"),
		tmp)))
	{
		Safe_Release(tmp);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_MiiEditor_UI_Layer()
{
	return S_OK;
}


HRESULT CPlace_MiiEditor::Ready_MiiEditor_Environment_Layer()
{
	return S_OK;
}

HRESULT CPlace_MiiEditor::Ready_MiiEditor_Camera_Layer()
{
	return S_OK;
}


CPlace_MiiEditor* CPlace_MiiEditor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_MiiEditor* pPlaceMain = new CPlace_MiiEditor(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_MiiEditor Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_MiiEditor::Free()
{
	CPlace::Free();
}
