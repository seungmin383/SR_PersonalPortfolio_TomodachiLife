#pragma once
#include "Game_Defines.h"

typedef struct tagApartInfoDesc
{
	/* 주민 이름 -> 로딩 */
	vector<_wstring >strMiiNames;

	/* 관리금 */
	int32_t iMoney;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["MiiNames"] = nlohmann::json::array();
		for (const auto& Name : strMiiNames)
		{
			Out["MiiNames"].push_back(WstrToStr(Name));
		}

		Out["Money"] = iMoney;

		return Out;
	}

	void FromJson(nlohmann::json& In)
	{
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
			In["Money"] = 5000;
	}

}APARTINFO_DESC;