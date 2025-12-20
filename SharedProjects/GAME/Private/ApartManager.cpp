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

void CApartManager::Add_FoodInventory(uint32_t iIndex)
{
	if (iIndex > 28 || iIndex < 0)
		return;
	int32_t Num = m_ApartInfo.iFoodInventory[iIndex];
	if (Num >= 99)
	{
		MSG_BOX("아이템이 99개임");
		return;
	}

	m_ApartInfo.iFoodInventory[iIndex]++;
}

void CApartManager::Minus_FoodInventoru(uint32_t iIndex)
{
	if (iIndex > 28 || iIndex < 0)
		return;
	int32_t Num = m_ApartInfo.iFoodInventory[iIndex];
	if (Num <= 0)
	{
		MSG_BOX("아이템이 0개임");
		return;
	}

	m_ApartInfo.iFoodInventory[iIndex]--;
}

void CApartManager::Free()
{
	CBase::Free();
}