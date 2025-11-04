#include "ConvertJson.h"

void Game::to_json(nlohmann::json& j, const MII_INFO& info)
{
    j = nlohmann::json{
        {"Gender", info.iGender},

        {"Face", {
            {"ShapeID", info.iFaceShapeID},
            {"MakeupID", info.iMakeupID},
            {"FaceShadowID", info.iFaceShadowID},
            {"SkinColorID", info.iSkinColorID}
        }},

        {"Hair", {
            {"HairID", info.iHairID},
            {"HairColorID", info.iHairColorID},
            {"isMirror", info.isMirror}
        }},

        {"Eyebrow", {
            {"EyebrowID", info.iEyebrowID},
            {"EyebrowColorID", info.iEyebrowColorID},
            {"State", nlohmann::json::array({info.arrEyebrowState[0], info.arrEyebrowState[1],
                                   info.arrEyebrowState[2], info.arrEyebrowState[3],
                                   info.arrEyebrowState[4]})}
        }},

        {"Eye", {
            {"EyeID", info.iEyeID},
            {"EyeColorID", info.iEyeColorID},
            {"State", nlohmann::json::array({info.arrEyeState[0], info.arrEyeState[1],
                                   info.arrEyeState[2], info.arrEyeState[3],
                                   info.arrEyeState[4]})}
        }},

        {"Nose", {
            {"NoseID", info.iNoseID},
            {"State", nlohmann::json::array({info.arrNoseState[0], info.arrNoseState[1]})}
        }},

        {"Mouth", {
            {"MouthID", info.iMouthID},
            {"MouthColorID", info.iMouthColorID},
            {"State", nlohmann::json::array({info.arrMouthState[0], info.arrMouthState[1], info.arrMouthState[2]})}
        }},

        {"Mole", {
            {"isMole", info.isMole},
            {"State", nlohmann::json::array({info.arrMoleState[0], info.arrMoleState[1], info.arrMoleState[2]})}
        }},

        {"Beard", {
            {"NoseBeardID", info.iNoseBeardID},
            {"NoseBeardColorID", info.iNoseBeardColorID},
            {"ChinBeardID", info.iChinBeardID},
            {"ChinBeardColorID", info.iChinBeardColorID}
        }},

        {"Height", info.fHeight},
        {"Fat", info.fFat},

        {"BloodType", info.iBloodType},
        {"FavorColor", info.iFavorColor},
        {"Birth", {
            {"Year", info.iBirthYear},
            {"Month", info.iBirthMonth},
            {"Day", info.iBirthDay}
        }},

        {"Personality", nlohmann::json::array()},
    };
    j["Personality"] = nlohmann::json::array();
    for (int i = 0; i < 5; ++i)
    {
        j["Personality"].push_back(info.iPersonality[i]);
    }
}

void Game::from_json(const nlohmann::json& j, MII_INFO& info)
{
    // Gender
    info.iGender = j.value("Gender", 0);

    // --- Face ---
    if (j.contains("Face")) {
        const auto& face = j["Face"];
        info.iFaceShapeID = face.value("ShapeID", 0);
        info.iMakeupID = face.value("MakeupID", 0);
        info.iFaceShadowID = face.value("FaceShadowID", 0);
        info.iSkinColorID = face.value("SkinColorID", 0);
    }

    // --- Hair ---
    if (j.contains("Hair")) {
        const auto& hair = j["Hair"];
        info.iHairID = hair.value("HairID", 0);
        info.iHairColorID = hair.value("HairColorID", 0);
        info.isMirror = hair.value("isMirror", false);
    }

    // --- Eyebrow ---
    if (j.contains("Eyebrow")) {
        const auto& brow = j["Eyebrow"];
        info.iEyebrowID = brow.value("EyebrowID", 0);
        info.iEyebrowColorID = brow.value("EyebrowColorID", 0);
        if (brow.contains("State")) {
            for (int i = 0; i < 5 && i < brow["State"].size(); ++i)
                info.arrEyebrowState[i] = brow["State"][i].get<char>();
        }
    }

    // --- Eye ---
    if (j.contains("Eye")) {
        const auto& eye = j["Eye"];
        info.iEyeID = eye.value("EyeID", 0);
        info.iEyeColorID = eye.value("EyeColorID", 0);
        if (eye.contains("State")) {
            for (int i = 0; i < 5 && i < eye["State"].size(); ++i)
                info.arrEyeState[i] = eye["State"][i].get<char>();
        }
    }

    // --- Nose ---
    if (j.contains("Nose")) {
        const auto& nose = j["Nose"];
        info.iNoseID = nose.value("NoseID", 0);
        if (nose.contains("State")) {
            for (int i = 0; i < 2 && i < nose["State"].size(); ++i)
                info.arrNoseState[i] = nose["State"][i].get<char>();
        }
    }

    // --- Mouth ---
    if (j.contains("Mouth")) {
        const auto& mouth = j["Mouth"];
        info.iMouthID = mouth.value("MouthID", 0);
        info.iMouthColorID = mouth.value("MouthColorID", 0);
        if (mouth.contains("State")) {
            for (int i = 0; i < 3 && i < mouth["State"].size(); ++i)
                info.arrMouthState[i] = mouth["State"][i].get<char>();
        }
    }

    // --- Glasses ---
    if (j.contains("Glasses")) {
        const auto& g = j["Glasses"];
        info.iGlassesID = g.value("GlassesID", 0);
        info.iGlassesColorID = g.value("GlassesColorID", 0);
    }

    // --- Mole ---
    if (j.contains("Mole")) {
        const auto& mole = j["Mole"];
        info.isMole = mole.value("isMole", false);
        if (mole.contains("State")) {
            for (int i = 0; i < 3 && i < mole["State"].size(); ++i)
                info.arrMoleState[i] = mole["State"][i].get<char>();
        }
    }

    // --- Beard ---
    if (j.contains("Beard")) {
        const auto& beard = j["Beard"];
        info.iNoseBeardID = beard.value("NoseBeardID", 0);
        info.iNoseBeardColorID = beard.value("NoseBeardColorID", 0);
        info.iChinBeardID = beard.value("ChinBeardID", 0);
        info.iChinBeardColorID = beard.value("ChinBeardColorID", 0);
    }

    // --- Body ---
    info.fHeight = j.value("Height", 0.0f);
    info.fFat = j.value("Fat", 0.0f);

    // --- Etc ---
    info.iBloodType = j.value("BloodType", 0);
    info.iFavorColor = j.value("FavorColor", 0);

    if (j.contains("Birth")) {
        const auto& b = j["Birth"];
        info.iBirthYear = b.value("Year", 0);
        info.iBirthMonth = b.value("Month", 0);
        info.iBirthDay = b.value("Day", 0);
    }

    if (j.contains("Personality"))
    {
        const auto& pers = j["Personality"];
        for (int i = 0; i < 5 && i < pers.size(); ++i)
            info.iPersonality[i] = pers[i].get<char>();
    }
}