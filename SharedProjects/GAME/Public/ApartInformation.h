#pragma once
#include "Game_Defines.h"

typedef struct tagApartInfoDesc
{
	/* 주민 이름 -> 로딩 */
	vector<_wstring >strMiiNames;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["MiiNames"] = nlohmann::json::array();

		for (const auto& Name : strMiiNames)
		{
			Out["MiiNames"].push_back(WstrToStr(Name));
		}

		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		for (const auto& j : In.at("MiiNames"))
		{
			_string name = j.get<_string>();
			strMiiNames.push_back(StrToWstr(name));
		}
	}

}APARTINFO_DESC;