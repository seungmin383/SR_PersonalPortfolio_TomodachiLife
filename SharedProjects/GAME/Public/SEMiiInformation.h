#pragma once
#include "Game_Defines.h"

struct MIIPROFILE_DESC
{
	_string strFirstName;
	_string strLastName;
	_string strNickName;
	_bool isMan;

	uint32_t iBlood;
	uint32_t iColor;
	uint32_t iBirthYear;
	uint32_t iBirthMonth;
	uint32_t iBirthDay;
	_bool isAdult;

	_float fLevel;
	_float fFlavor[9];
	uint32_t iFullness;

	uint32_t iRoomIndex;
	
	vector<uint32_t> iFriendIndexList;

	uint32_t iNeeds;

	int32_t iPersonal;

	nlohmann::json ToJson()const
	{
		nlohmann::json Out;

		Out["FirstName"]	= strFirstName;
		Out["LastName"]		= strLastName;
		Out["NickName"]		= strNickName;
		Out["isMan"]		= isMan;

		Out["Blood"]		= iBlood;
		Out["Color"]		= iColor;
		Out["BirthYear"]	= iBirthYear;
		Out["BirthMonth"]	= iBirthMonth;
		Out["BirthDay"]		= iBirthDay;
		Out["isAdult"]		= isAdult;


		Out["Level"]		= fLevel;

		Out["Flavor"]		= nlohmann::json::array();
		for (uint32_t i = 0; i < 9; i++)
			Out["Flavor"].push_back(fFlavor[i]);

		Out["Fullness"]		= iFullness;
		Out["RoomIndex"]	= iRoomIndex;

		Out["FriendIndexList"] = nlohmann::json::array();
		for(const auto& i : iFriendIndexList)
			Out["FriendIndexList"].push_back(i);


		Out["Needs"] = iNeeds;
		Out["Personal"] = iPersonal;
		return Out;
	}

	void FromJson(const nlohmann::json& In)
	{
		strFirstName	= In.at("FirstName").get<_string>();
		strLastName		= In.at("LastName").get<_string>();
		strNickName		= In.at("NickName").get<_string>();
		isMan			= In.at("isMan").get<_bool>();

		iBlood			= In.at("Blood").get<uint32_t>();
		iColor			= In.at("Color").get<uint32_t>();
		iBirthYear		= In.at("BirthYear").get<uint32_t>();
		iBirthMonth		= In.at("BirthMonth").get<uint32_t>();
		iBirthDay		= In.at("BirthDay").get<uint32_t>();
		isAdult			= In.at("isAdult").get<_bool>();

		fLevel			= In.at("Level").get<_float>();

		const auto& Arr = In.at("Flavor");
		for (uint32_t i = 0; i < 9; i++)
			fFlavor[i] = Arr.at(i).get<_float>();

		iFullness		= In.at("Fullness").get<uint32_t>();
		iRoomIndex		= In.at("RoomIndex").get<uint32_t>();

		iFriendIndexList.clear();
		const auto& FriendList = In.at("FriendIndexList");
		for (const auto& i : FriendList)
			iFriendIndexList.push_back(i.get<uint32_t>());

		iNeeds = In.at("Needs").get<uint32_t>();
		iPersonal = In.at("Personal").get<int32_t>();
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

