#include "Place_ShowMii.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "WorldCamera.h"
#include "UICamera.h"

/* Manager */
#include "MiiManager.h"
#include "ApartManager.h"
#include "GameDirector.h"

/* Actor */
#include "Mii.h"
#include "MiiEditorBG.h"

/* UI */
#include "BlueBackGround.h"
#include "Personal_Base_BackGround.h"
#include "Personal_Sub_BackGround.h"
#include "Option_Button.h"

/* strgy */
#include "SaveMiiData_Strgy.h"
#include "ChangePlace_Strategy.h"

CPlace_ShowMii::CPlace_ShowMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_ShowMii::Initialize()
{
	if (FAILED(Ready_Actor_Layer()))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;
	if (FAILED(Ready_Camera_Layer()))
		return E_FAIL;


	if (CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal > 0)
		m_pGameInstance->SoundManager_Begin_Sound(TEXT("SFX_Personal1"), SOUND_CHANNEL::SFX, 1.f);
	else
		m_pGameInstance->SoundManager_Begin_Sound(TEXT("SFX_Personal1"), SOUND_CHANNEL::SFX, 1.f);

	if (FAILED(m_pGameInstance->SoundManager_Begin_BGM(TEXT("BGM_MiiEditor"), 1.f)))
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_ShowMii::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_ShowMii::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("완성된 mii를 보여줘"));
	return S_OK;
}

HRESULT CPlace_ShowMii::Render_Late()
{
	SetWindowText(g_hWnd, TEXT("완성된 mii를 보여줘"));
	return S_OK;
}

HRESULT CPlace_ShowMii::Ready_Actor_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);


	// ProtoTag_Actor_Mii
	{
		CMii::MII_DESC MiiDesc = {};
		MiiDesc.vPos = XMFLOAT4{ 0.f, 0.f, 0, 1.f };
		MiiDesc.fCX = 1.f;
		MiiDesc.fCY = 1.f;
		MiiDesc.fCZ = 1.f;
		MiiDesc.fSpeedPerSecond = 0.f;
		MiiDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		MiiDesc.fRotationPerSecond = XMConvertToRadians(90.f);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC),
			TEXT("ProtoTag_Actor_Mii"), &MiiDesc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID,
			TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;
	}


	// ProtoTag_Actor_EditorBG
	{
		CMiiEditorBG::MIIEDITORBG_DESC Desc = {};
		Desc.vPos = XMFLOAT4{ 0.f, 0.f, 5.f, 0.f };
		Desc.fCX = 0.05f;
		Desc.fCY = 0.05f;
		Desc.fCZ = 0.05f;
		Desc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		Desc.fSpeedPerSecond = 10.f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_EditorBG"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;

		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_NONBLEND;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_EditorBG"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_Actor"), tmp)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CPlace_ShowMii::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);

	if (FAILED(Layer_UI_PersonalBase_BackGround()))
		return E_FAIL;

	if (FAILED(Layer_UI_PersonalSub_BackGround()))
		return E_FAIL;


	//LayerTag_UI_Save_Button
	{
		COption_Button::OPTIONBUTTON_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 440.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 300.f;
		Desc.fCY = 80.f;
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.strText = TEXT("OK");
		Desc.strSoundTag = TEXT("SFX_Button_Click_Enter");

		Desc.pClicked_Strgy = CSaveMiiData_Strgy::Create(m_pDevice, m_pContext);
		Desc.pSelected_Strgy = CChangePlace_Strgy::Create(m_pDevice, m_pContext, PLACEID::ISLAND);
		Desc.pSelectedClick_Strgy = CNull_Strgy::Create(m_pDevice, m_pContext);

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_Option_Button"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_Save_Button"), tmp)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->ButtonManager_Add_Button(iPlaceID, TEXT("LayerTag_UI_Save_Button"), static_cast<CButton*>(tmp))))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CPlace_ShowMii::Ready_Environment_Layer()
{
	CObject* tmp = { nullptr };
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);

	LIGHT_DESC Desc = {};
	int a = CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal;

	if (CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal >= 0)
	{
		Desc.eType = LIGHT_TYPE::DIRECTIONAL;
		Desc.vDir = _float4(1.8f, -2.f, 3.f, 0.f);
		Desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.0f);
		Desc.vAmbient = _float4(0.9f, 0.3f, 0.3f, 1.0f);
		Desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.0f);


	}
	else
	{
		Desc.eType = LIGHT_TYPE::DIRECTIONAL;
		Desc.vDir = _float4(1.8f, -2.f, 3.f, 0.f);
		Desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.0f);
		Desc.vAmbient = _float4(0.3f, 0.9f, 0.3f, 1.0f);
		Desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.0f);

	}

	if (FAILED(m_pGameInstance->LightManager_Add_Light(Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlace_ShowMii::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 0.f, 6.f, -15.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 0.f, 2.5f, 0.f, 1.f }, XMConvertToRadians(20.f), 1.f);

	return S_OK;
}

HRESULT CPlace_ShowMii::Layer_UI_PersonalBase_BackGround()
{
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);
	CObject* tmp = { nullptr };

	// ProtoTag_UI_PersonalBase_BackGround
	{
		CPersonal_Base_BackGround::PERSONALBASEBACKGROUND_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 900.f;
		Desc.fCY = 220.f;
		Desc.fX = 440.f;
		Desc.fY = 130.f;
		Desc.fZ = 0.5f;
		Desc.TargetFadeTime = 0.3f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::SHOW_MII), TEXT("ProtoTag_UI_PersonalBase_BackGround"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_PersonalBase_BackGround"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlace_ShowMii::Layer_UI_PersonalSub_BackGround()
{
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);
	CObject* tmp = { nullptr };

	// ProtoTag_UI_PersonalSub_BackGround
	{
		CPersonal_Sub_BackGround::PERSONALSUBBACKGROUND_DESC Desc = {};
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;
		Desc.fCX = 860.f;
		Desc.fCY = 220.f;
		Desc.fX = 540.f;
		Desc.fY = 280.f;
		Desc.fZ = 0.5f;
		Desc.TargetFadeTime = 0.3f;

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::SHOW_MII), TEXT("ProtoTag_UI_PersonalSub_BackGround"), &Desc);
		if (nullptr == tmp)
			return E_FAIL;

		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(iPlaceID, TEXT("LayerTag_UI_PersonalBase_BackGround"), tmp)))
			return E_FAIL;
	}

	return S_OK;
}

CPlace_ShowMii* CPlace_ShowMii::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlace_ShowMii* pPlaceMain = new CPlace_ShowMii(pDevice, pContext);

	if (FAILED(pPlaceMain->Initialize()))
	{
		MSG_BOX("ErrCode : Place_Apart Create Failed");
		Safe_Release(pPlaceMain);
	}
	return pPlaceMain;
}

void CPlace_ShowMii::Free()
{
	CPlace::Free();
}
