#pragma once
#include "Game_Defines.h"

typedef struct tagApartInfoDesc
{
	/* 주민 이름 -> 로딩 */
	vector<_wstring> strMiiNames;

	/* 관리금 */
	int32_t iMoney;

	/* 음식 인벤토리 */
	vector<uint32_t> iFoodInventory;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["MiiNames"] = nlohmann::json::array();
		for (const auto& Name : strMiiNames)
		{
			Out["MiiNames"].push_back(WstrToStr(Name));
		}

		Out["Money"] = iMoney;

		Out["FoodInventory"] = nlohmann::json::array();
		for (uint32_t i : iFoodInventory)
		{
			Out["FoodInventory"].push_back(i);
		}

		return Out;
	}

	void FromJson(nlohmann::json& In)
	{
		strMiiNames.clear();
		iFoodInventory.clear();
		iFoodInventory.resize(29, 0);

		if (In.contains("MiiNames"))
		{
			for (const auto& j : In.at("MiiNames"))
			{
				_string name = j.get<_string>();
				strMiiNames.push_back(StrToWstr(name));
			}
		}
		else
			In["MiiNames"] = nlohmann::json::array();

		if (In.contains("Money"))
			iMoney = In.at("Money").get<int32_t>();
		else
		{
			In["Money"] = 5000;
			iMoney = 5000;
		}

		if (In.contains("FoodInventory"))
		{
			uint32_t iIndex = { 0 };

			for (const auto j : In.at("FoodInventory"))
			{
				uint32_t foodCnt = j.get<uint32_t>();
				iFoodInventory[iIndex++] = (foodCnt);
			}
		}
	}

}APARTINFO_DESC;