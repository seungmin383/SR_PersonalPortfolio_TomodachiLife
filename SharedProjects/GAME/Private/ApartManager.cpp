#include "ApartManager.h"

#define APARTDATA_FILEPATH "../../SharedProjects/GAME/GameData/ApartData.json"

IMPLEMENT_SINGLETON(CApartManager)

CApartManager::CApartManager()
{
}

HRESULT CApartManager::Save_ApartData()
{
	ofstream Out(APARTDATA_FILEPATH);
	if (!Out.is_open())
		return E_FAIL;

	nlohmann::json j = m_ApartInfo.ToJson();
	Out << j.dump(4);

	MSG_BOX("Success Save : Apart");


	return S_OK;
}

HRESULT CApartManager::Load_ApartData()
{
	ifstream In(APARTDATA_FILEPATH);
	if (!In.is_open())
		return E_FAIL;

	if (In.peek() == std::ifstream::traits_type::eof())
	{
		return S_OK;
	}

	nlohmann::json j;
	In >> j;
	m_ApartInfo.FromJson(j);

	return S_OK;
}

void CApartManager::Free()
{
	CBase::Free();
}