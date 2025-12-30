#include "SaveMiiData_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "MiiManager.h"
#include "ApartManager.h"

CSaveMiiData_Strgy::CSaveMiiData_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CSaveMiiData_Strgy::Initialize()
{
	return S_OK;
}

void CSaveMiiData_Strgy::OnClick()
{
	_wstring strNickName = StrToWstr(CMiiManager::GetInstance()->Get_Edit_Profile().strNickName);
	_wstring strFilePath = L"../../SharedProjects/GAME/GameData/MiiData/" + strNickName + L".json";
	
	/* 주민 이름 저장 */
	CApartManager::GetInstance()->Add_ApartInfo_NewMii(strNickName);

	CMiiManager::GetInstance()->End_Edit();
	CApartManager::GetInstance()->Save_ApartData();

	CMiiManager::GetInstance()->Load_AllMiiData();
}

CSaveMiiData_Strgy* CSaveMiiData_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSaveMiiData_Strgy* pInstance = new CSaveMiiData_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ErrCode : CSaveMiiData_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSaveMiiData_Strgy::Free()
{
	IOnClick::Free();
}
