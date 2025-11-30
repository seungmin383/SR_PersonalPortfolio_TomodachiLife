#include "SaveChangePlace_Strategy.h"
#include "MiiManager.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "ApartManager.h"

CSaveChangePlace_Strategy::CSaveChangePlace_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSaveChangePlace_Strategy::Initialize()
{
	return S_OK;
}

void CSaveChangePlace_Strategy::OnClick()
{
	_wstring nickName = StrToWstr(CMiiManager::GetInstance()->Get_Profile().strNickName);
	_wstring filePath = { L"../../SharedProjects/GAME/GameData/MiiData/" };
	filePath += nickName;
	filePath += L".json";

	/* 주민 이름 저장 */
	CApartManager::GetInstance()->Get_ApartInfo().strMiiNames.push_back(nickName);

	CMiiManager::GetInstance()->Save_MiiData(filePath);
	CApartManager::GetInstance()->Save_ApartData();

	m_pGameInstance->PlaceManager_Change_Place(
		static_cast<uint32_t>(PLACEID::LOADING),
		CPlace_Loading::Create(m_pDevice, m_pContext,PLACEID::ISLAND));

	CMiiManager::GetInstance()->Set_EditFin(TRUE);
}

CSaveChangePlace_Strategy* CSaveChangePlace_Strategy::Create(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSaveChangePlace_Strategy* pInstance = new CSaveChangePlace_Strategy(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CSaveChangePlace_Strategy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSaveChangePlace_Strategy::Free()
{
	IOnClick::Free();
}

