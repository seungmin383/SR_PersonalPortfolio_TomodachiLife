#pragma once
#include "Game_Defines.h"

struct MIIPROFILE_DESC
{
	_string strFirstName;
	_string strLastName;
	_string strNickName;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["FirstName"] = strFirstName;
		Out["LastName"] = strLastName;
		Out["NickName"] = strNickName;
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		strFirstName = In.at("FirstName").get<_string>();
		strLastName = In.at("LastName").get<_string>();
		strNickName = In.at("NickName").get<_string>();
	}
};

struct MIIHEADINFO_DESC
{
	uint32_t iIndex;
	XMFLOAT4 vSkinColor;

	
	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["Index"] = iIndex;
		Out["SkinColor"] = {vSkinColor.x, vSkinColor.y, vSkinColor.z, vSkinColor.w};
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		iIndex = In.at("Index").get<uint32_t>();
		auto j = In.at("SkinColor");
		vSkinColor = XMFLOAT4(
			j[0].get<float>(), 
			j[1].get<float>(), 
			j[2].get<float>(),
			j[3].get<float>());
	}
};

struct MIIHAIRINFO_DESC
{
	uint32_t iIndex;
	XMFLOAT4 vHairColor;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["Index"] = iIndex;
		Out["HairColor"] = { vHairColor.x, vHairColor.y, vHairColor.z, vHairColor.w };
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		iIndex = In.at("Index").get<uint32_t>();
		auto j = In.at("HairColor");
		vHairColor = XMFLOAT4(
			j[0].get<float>(),
			j[1].get<float>(),
			j[2].get<float>(),
			j[3].get<float>());
	}
};

struct MIIEYEBROWINFO_DESC
{
	uint32_t iIndex;
	XMFLOAT4 vEyebrowColor;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["Index"] = iIndex;
		Out["EyebrowColor"] = { vEyebrowColor.x,vEyebrowColor.y, vEyebrowColor.z,vEyebrowColor.w };

		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		iIndex = In.at("Index").get<uint32_t>();		
		auto j = In.at("EyebrowColor");
		vEyebrowColor = XMFLOAT4(
			j[0].get<float>(),
			j[1].get<float>(),
			j[2].get<float>(),
			j[3].get<float>());
	}
};

struct MIIEYESINFO_DESC
{
	uint32_t iIndex;
	_float fBlinkTime;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["Index"] = iIndex;
		Out["BlinkTime"] = fBlinkTime;
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		iIndex = In.at("Index").get<uint32_t>();
		fBlinkTime = In.at("BlinkTime").get<_float>();
	}
};

struct MIINOSEINFO_DESC
{
	uint32_t iIndex;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["Index"] = iIndex;
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		iIndex = In.at("Index").get<uint32_t>();
	}
};

struct MIIMOUTHINFO_DESC
{
	uint32_t iIndex;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["Index"] = iIndex;
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		iIndex = In.at("Index").get<uint32_t>();
	}
};
typedef struct tagMiiInfo
{
	MIIPROFILE_DESC		MiiProfile{};
	MIIHEADINFO_DESC	HeadInfo {};
	MIIHAIRINFO_DESC	HairInfo {};
	MIIEYEBROWINFO_DESC EyebrowInfo{};
	MIIEYESINFO_DESC	EyesInfo{};
	MIINOSEINFO_DESC	NoseInfo{};
	MIIMOUTHINFO_DESC	MouthInfo{};

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["MiiProfile"]	= MiiProfile.ToJson();
		Out["HeadInfo"]		= HeadInfo.ToJson();
		Out["HairInfo"]		= HairInfo.ToJson();
		Out["EyebrowInfo"]	= EyebrowInfo.ToJson();
		Out["EyesInfo"]		= EyesInfo.ToJson();
		Out["NoseInfo"]		= NoseInfo.ToJson();
		Out["MouthInfo"]	= MouthInfo.ToJson();
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		MiiProfile.FromJson(In.at("MiiProfile"));
		HeadInfo.FromJson(In.at("HeadInfo"));
		HairInfo.FromJson(In.at("HairInfo"));
		EyebrowInfo.FromJson(In.at("EyebrowInfo"));
		EyesInfo.FromJson(In.at("EyesInfo"));
		NoseInfo.FromJson(In.at("NoseInfo"));
		MouthInfo.FromJson(In.at("MouthInfo"));
	}


}MIIINFO_DESC;

