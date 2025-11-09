#pragma once
#include"Game_Defines.h"

namespace Game
{
	typedef struct tagMiiInfo
	{
		// Gender
		char iGender;

		// Face
		char iFaceShapeID = {};
		char iMakeupID = {};
		char iFaceShadowID = {};
		char iSkinColorID = {};

		// Hair
		char iHairID = {};
		char iHairColorID = {};
		bool isMirror = { false };

		// Eyebrow
		char iEyebrowID = {};
		char iEyebrowColorID = {};
		char arrEyebrowState[5] = {};

		// Eye
		char iEyeID = {};
		char iEyeColorID = {};
		char arrEyeState[5] = {};

		// Nose
		char iNoseID = {};
		char arrNoseState[2] = {};

		// Mouth 
		char iMouthID = {};
		char iMouthColorID = {};
		char arrMouthState[3] = {};

		// Glasses
		char iGlassesID = {};
		char iGlassesColorID = {};

		// Nose Beard
		char iNoseBeardID = {};
		char iNoseBeardColorID = {};

		// Mole
		bool isMole = { false };
		char arrMoleState[3] = {};

		// chin Beard
		char iChinBeardID = {};
		char iChinBeardColorID = {};

		// Height
		float fHeight = {};

		// Fat
		float fFat = {};

		wchar_t strTopName = {};
		wchar_t strName = {};
		wchar_t strNickname = {};

		char iBloodType = {};
		char iFavorColor = {};
		short iBirthYear = {};
		char iBirthMonth = {};
		char iBirthDay = {};

		char iPersonality[5] = {};

	}MII_INFO;




	typedef struct tagMeshInfo
	{

	}MESH_INFO	;

	typedef struct tagModelInfo
	{
		_wstring strProtoTag;

		MESH_INFO eMeshInfo;

	}MODEL_INFO	;

    void to_json(nlohmann::json& j, const MII_INFO& info);
    void to_json(nlohmann::json& j, const MODEL_INFO& info);
   
	void from_json(const nlohmann::json& j, MII_INFO& info);
	void from_json(const nlohmann::json& j, MODEL_INFO& info);

}