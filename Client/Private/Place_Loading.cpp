#include "Place_Loading.h"
#include "Loader.h"
#include "GameInstance.h"

#include "Place_Main.h"
#include "Place_Setting.h"
#include "Place_Island.h"
#include "Place_Apart.h"
#include "Place_CityHall.h"
#include "Place_Developer.h"

CPlace_Loading::CPlace_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_Loading::Initialize(
	PLACEID NextPlaceID)
{
	m_eNextPlaceID = NextPlaceID;
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextPlaceID);

	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

int32_t CPlace_Loading::Update(_float fDT)
{
	if (m_pLoader->isFinished())
	{
		CPlace* pPlace = { nullptr };

		switch (m_eNextPlaceID)
		{
		case PLACEID::STATIC:
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

		if (SUCCEEDED(m_pGameInstance->PlaceManager_Change_Place(static_cast<uint32_t>(m_eNextPlaceID), pPlace)))
			return 1;

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

CPlace_Loading* CPlace_Loading::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	PLACEID NextPlaceID)
{
	CPlace_Loading* pPlaceLoading = new CPlace_Loading(pDevice, pContext);

	if (FAILED(pPlaceLoading->Initialize(NextPlaceID)))
	{
		MSG_BOX("ErrCode : Loading Place Create Failed");
		Safe_Release(pPlaceLoading);
		return nullptr;
	}

	return pPlaceLoading;
}

void CPlace_Loading::Free()
{
	Safe_Release(m_pLoader);

	CPlace::Free();
}
