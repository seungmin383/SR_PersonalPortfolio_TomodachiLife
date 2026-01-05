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

	uint32_t iIndex = {};
	uint32_t iNumMii = static_cast<uint32_t>(m_Mii_Informations.size());

	for (auto& info : m_Mii_Informations)
	{
		/* 나랑 친구인 mii 인덱스들을 가져옴 */
		const auto& iArr = info.MiiProfile.iFriendIndexList;
		_bool isMan = info.MiiProfile.isMan;

		/* 나랑 친구가 아닌 애들을 저장할꺼임*/
		vector<uint32_t>NotFriendList;

		/* 나랑 친구고 이성인 애들 저장할거임 */
		vector<uint32_t> LoveList;

		/* 모든 주민 순회 */

		for (uint32_t i = 0; i < iNumMii; ++i)
		{
			_bool isFriend = { FALSE };
			_bool CanLove = { FALSE };

			/* 나면 빼기 */
			if (iIndex == i)
				continue;

			/* 나랑 친구인 애들 인덱스 j */
			for (uint32_t j : iArr)
			{
				if (j == i)
				{
					_bool a = m_Mii_Informations[j].MiiProfile.isMan;
					_bool b = info.MiiProfile.isMan;

					if (m_Mii_Informations[j].MiiProfile.isMan != info.MiiProfile.isMan)
						CanLove = TRUE;

					isFriend = TRUE;
					break;
				}
			}

			if (!isFriend)
				NotFriendList.push_back(i);
			if (CanLove)
				LoveList.push_back(i);
		}

		iIndex++;

		uint32_t iRandNeeds = rand() % static_cast<int32_t>(NEEDS::END);
		info.MiiProfile.iNeeds = iRandNeeds;
	}

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
		m_EditDesc.MiiProfile.isMan = TRUE;
	}
	else
	{
		m_EditDesc.HeadInfo.iIndex		= 0;
		m_EditDesc.HairInfo.iIndex		= 0;
		m_EditDesc.EyebrowInfo.iIndex	= 79;
		m_EditDesc.EyesInfo.iIndex		= 1;
		m_EditDesc.NoseInfo.iIndex		= 60;
		m_EditDesc.MouthInfo.iIndex		= 101;
		m_EditDesc.MiiProfile.isMan = FALSE;
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