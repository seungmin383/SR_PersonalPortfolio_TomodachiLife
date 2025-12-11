#include "Place_Loading.h"
#include "Loader.h"
#include "GameInstance.h"

#include "Place_Island.h"
#include "Place_Apart.h"
#include "Place_CityHall.h"
#include "Place_Developer.h"
#include "Place_MiiEditor.h"
#include "Place_Room.h"

#include "BackGround.h"

CPlace_Loading::CPlace_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Loading::Initialize(
	PLACEID NextPlaceID)
{
	m_eNextPlaceID = NextPlaceID;
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextPlaceID);

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;	
	if (FAILED(Ready_Camera_Layer()))
		return E_FAIL;

	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_Loading::Update(_float fDT)
{
	m_fTimer += fDT;

	if (m_pLoader->isFinished() && m_fTimer > 1.f)
	{
		CPlace* pPlace = { nullptr };
		m_fTimer = 0.f;
		switch (m_eNextPlaceID)
		{
		case PLACEID::STATIC:
			break;

		case PLACEID::LOADING:
			break;

		case PLACEID::ISLAND:
			pPlace = CPlace_Island::Create(m_pDevice, m_pContext);
			break;

		case PLACEID::APART:
			pPlace = CPlace_Apart::Create(m_pDevice, m_pContext);
			break;

		case PLACEID::CITYHALL:
			pPlace = CPlace_CityHall::Create(m_pDevice, m_pContext);
			break;

		case PLACEID::MII_EDITOR:
			pPlace = CPlace_MiiEditor::Create(m_pDevice, m_pContext);
			break;

		case PLACEID::ROOM:
			pPlace = CPlace_Room::Create(m_pDevice, m_pContext);
			break;

		case PLACEID::DEVELOPER:
			pPlace = CPlace_Developer::Create(m_pDevice, m_pContext);
			break;

		case PLACEID::END:
			break;
		default:
			break;
		}

		if (nullptr == pPlace)
			return 1;

		m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(m_eNextPlaceID), pPlace);
		return 1 ;

		MSG_BOX("ErrCode : 왜 여기까지 왔지? ");
	}

	return 0;
}

HRESULT CPlace_Loading::Render_Priority()
{
	SetWindowText(g_hWnd, TEXT("로딩레벨입니다."));
	return S_OK;
}

HRESULT CPlace_Loading::Render_Late()
{
	return S_OK;
}


HRESULT CPlace_Loading::Ready_UI_Layer()
{
	CObject* tmp = { nullptr };

	{
		CBackGround::BACKGROUND_DESC Desc = {};
		Desc.fX = 480.f;
		Desc.fY = 240.f;
		Desc.fZ = 0.5f;
		Desc.fCX = 960.f;
		Desc.fCY = 480.f;
		Desc.fSpeedPerSecond = 0.f;
		Desc.fRotationPerSecond = XMConvertToRadians(0.f);
		Desc.eRenderGroup = RENDER_GROUP::BOTTOM_UI;

		Desc.eTexturePlaceID = PLACEID::STATIC;
		Desc.strTextureTag = TEXT("ProtoTag_Component_LoadingTexture");

		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC),TEXT("ProtoTag_UI_LoadingBG"),&Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::LOADING),
			TEXT("LayerTag_UI"),tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}

		Desc.eRenderGroup = RENDER_GROUP::TOP_UI;
		tmp = m_pGameInstance->Proto_Get_Object_Clone(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_UI_LoadingBG"), &Desc);
		if (FAILED(m_pGameInstance->ObjectManager_Add_Object_in_Layer(static_cast<uint32_t>(PLACEID::LOADING),
			TEXT("LayerTag_UI"), tmp)))
		{
			Safe_Release(tmp);
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPlace_Loading::Ready_Camera_Layer()
{
	m_pGameInstance->CameraManager_Reset_State(TEXT("World_Camera"));
	m_pGameInstance->CameraManager_Set_Position(TEXT("World_Camera"), XMVECTOR{ 2.f, 5.f, -30.f, 1.f });
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMVECTOR{ 2.f, 0.f, 0.f, 1.f }, XMConvertToRadians(45.f), 1.f);

	return S_OK;
}

CPlace_Loading* CPlace_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PLACEID NextPlaceID)
{
	CPlace_Loading* pInstance = new CPlace_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(NextPlaceID)))
	{
		MSG_BOX("ErrCode : CPlace_Loading Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlace_Loading::Free()
{
	Safe_Release(m_pLoader);

	CPlace::Free();
}
