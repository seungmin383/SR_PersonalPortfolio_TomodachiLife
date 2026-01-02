#include "MiiManager.h"
#include "ApartManager.h"

IMPLEMENT_SINGLETON(CMiiManager)

#define SAVEDATA_FOLDER_PATH L"../../SharedProjects/GAME/GameData/MiiData/" 
CMiiManager::CMiiManager()
{
}

HRESULT CMiiManager::Initialize()
{
	if (FAILED(Load_AllMiiData()))
		return E_FAIL;	

	for (auto& info : m_Mii_Informations)
		info.MiiProfile.iNeeds = static_cast<uint32_t>(NEEDS::BE_FRIEND);
		//info.MiiProfile.iNeeds = rand() % static_cast<int32_t>(NEEDS::END);

	return S_OK;
}

/* 편집을 시작할 때 꼭 부를 것 */
HRESULT CMiiManager::Begin_Edit(_bool isMan)
{
	if (!m_isEdit)
		m_isEdit = TRUE;
	else
	{
		MSG_BOX("편집중입니다.");
		return E_FAIL;
	}

	if (isMan)
	{
		m_EditDesc.HeadInfo.iIndex		= 0;
		m_EditDesc.HairInfo.iIndex		= 1;
		m_EditDesc.EyebrowInfo.iIndex	= 78;
		m_EditDesc.EyesInfo.iIndex		= 0;
		m_EditDesc.NoseInfo.iIndex		= 60;
		m_EditDesc.MouthInfo.iIndex		= 101;
	}
	else
	{
		m_EditDesc.HeadInfo.iIndex		= 0;
		m_EditDesc.HairInfo.iIndex		= 0;
		m_EditDesc.EyebrowInfo.iIndex	= 79;
		m_EditDesc.EyesInfo.iIndex		= 1;
		m_EditDesc.NoseInfo.iIndex		= 60;
		m_EditDesc.MouthInfo.iIndex		= 101;
	}

	return S_OK;
}

/* 편집을 종료할 때 꼭 부를 것 */
HRESULT CMiiManager::End_Edit()
{
	if (!m_isEdit)
	{
		MSG_BOX("편집중이 아닙니다.");
		return E_FAIL;
	}


	_wstring strFilePath = SAVEDATA_FOLDER_PATH + StrToWstr(m_EditDesc.MiiProfile.strNickName) + L".json";

	ofstream Out(strFilePath);
	if (!Out.is_open())
		return E_FAIL;

	nlohmann::json j = m_EditDesc.ToJson();
	Out << j.dump(4);

	MSG_BOX("Success Save : MiiData");

	m_Mii_Informations.push_back(m_EditDesc);
	m_isEdit = FALSE; 
	return S_OK;
}

HRESULT CMiiManager::Save_MiiData(const _wstring& strNickName)
{
	_wstring strFilePath = SAVEDATA_FOLDER_PATH + strNickName + L".json";

	ofstream Out(strFilePath);
	if (!Out.is_open())
		return E_FAIL;

	nlohmann::json j = m_EditDesc.ToJson();
	Out << j.dump(4);

	MSG_BOX("Success Save : MiiData");


	return S_OK;
}

HRESULT CMiiManager::Load_MiiData(__out MIIINFO_DESC& Out, const _wstring& strNickName)
{
	_wstring strFilePath = SAVEDATA_FOLDER_PATH + strNickName + L".json";

	ifstream In(strFilePath);
	if (!In.is_open())
		return E_FAIL;

	nlohmann::json j;
	In >> j;
	Out.FromJson(j);
	return S_OK;
}

HRESULT CMiiManager::Save_AllMiiData()
{
	for (uint32_t i = 0; i < CApartManager::GetInstance()->Get_ApartInfo_NumMii(); ++i)
	{
		_wstring strFilePath = SAVEDATA_FOLDER_PATH + CApartManager::GetInstance()->Get_ApartInfo_MiiName(i) + L".json";
		ofstream Out(strFilePath);

		if (!Out.is_open())
			return E_FAIL;

		nlohmann::json j = m_Mii_Informations[i].ToJson();
		Out << j.dump(4);
	}


	MSG_BOX("Success Save : MiiData");
	return S_OK;
}

HRESULT CMiiManager::Load_AllMiiData()
{
	uint32_t iNumMii = CApartManager::GetInstance()->Get_ApartInfo_NumMii();

	for (uint32_t i = 0; i < iNumMii; ++i)
	{
		_wstring strNickName = CApartManager::GetInstance()->Get_ApartInfo_MiiName(i);
		_wstring strFilePath = L"../../SharedProjects/GAME/GameData/MiiData/" + strNickName + L".json";

		ifstream In(strFilePath);
		if (!In.is_open())
			return E_FAIL;

		nlohmann::json j;
		In >> j;
		MIIINFO_DESC Desc = {};
		Desc.FromJson(j);

		m_Mii_Informations.push_back(Desc);
	}
	return S_OK;
}


void CMiiManager::Free()
{
	CBase::Free();
}