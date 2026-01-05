#include "AnimationManager.h"
#include "SEAnimation.h"

#define ANIM_BINARY_FOLDER_PATH L"../../SharedProjects/GAME/Asset/Animations"

#define ANIM_MACRO_DATA_FILE_PATH L"../../SharedProjects/GAME/GameData/AnimMacroData.json" 

CAnimationManager::CAnimationManager()
{
}

HRESULT CAnimationManager::Initialize()
{
    /* 애니메이션들 */
    vector<SE_ANIMATION> AnimationDescs;
    for (const auto& entry : filesystem::directory_iterator(ANIM_BINARY_FOLDER_PATH))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() != L".bin")
            continue;

        ifstream in(entry.path(), ios::binary);
        if (!in.is_open())
            continue;

        SE_ANIMATION seAnim = {};
        seAnim.ReadSEAnimationFile(in);
        AnimationDescs.push_back(seAnim);
    }

    for (auto& Desc : AnimationDescs)
    {
        CSEAnimation* pAnimation = CSEAnimation::Create(Desc);
        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.emplace(StrToWstr(Desc.strName), pAnimation);
    }

    /* 애니메이션 매크로 */
    {
        ifstream in(ANIM_MACRO_DATA_FILE_PATH);

        if (!in.is_open())
            return E_FAIL;

        nlohmann::json j;
        in >> j;

        m_AnimationMacros.clear();

        /* json 순회 */
        for (auto iter = j.begin(); iter != j.end(); ++iter)
        {
            const _wstring Tag = StrToWstr(iter.key());

            const nlohmann::json& Value = iter.value();
            vector<_wstring> AnimTags;
            vector<_float> AnimSpeeds;
            vector<uint32_t> AnimLoopTime;

            if (!Value.is_array())
                continue;

            /* 애니메이션 태그들 */
            for (const auto& obj : Value)
            {
                if (!obj.is_object())
                    continue;

                if (!obj.contains("AnimTag"))
                    continue;

                if (!obj["AnimTag"].is_string())
                    continue;

                AnimTags.push_back(StrToWstr(obj["AnimTag"].get<_string>()));
            }

            /* 애니메이션 속도 */
            for (const auto& obj : Value)
            {
                if (!obj.is_object())
                    continue;

                if (!obj.contains("AnimSpeed"))
                    continue;

                if (!obj["AnimSpeed"].is_number())
                    continue;

                AnimSpeeds.push_back(obj["AnimSpeed"].get<_float>());
            }

            /* 애니메이션 반복 횟수 */
            for (const auto& obj : Value)
            {
                if (!obj.is_object())
                    continue;

                if (!obj.contains("LoopTime"))
                    continue;

                if (!obj["LoopTime"].is_number())
                    continue;

                AnimLoopTime.push_back(obj["LoopTime"].get<uint32_t>());
            }

            m_AnimationMacros.emplace(Tag, move(AnimTags));
            m_AnimationSpeeds.emplace(Tag, move(AnimSpeeds));
            m_AnimationLoopTime.emplace(Tag, move(AnimLoopTime));
        }

        return S_OK;
    }
}

CSEAnimation* CAnimationManager::Find_Animation(const _wstring& strAnimationTag)
{
    auto iter = m_Animations.find(strAnimationTag);

    if (m_Animations.end() == iter)
    {
        MSG_BOX("ErrCode : No Animation");
        return nullptr;
    }

    return iter->second->Clone();
}

vector<_wstring>* CAnimationManager::Find_AnimationMacro(const _wstring& strMacroTag)
{
    auto iter = m_AnimationMacros.find(strMacroTag);

    if(iter == m_AnimationMacros.end())
        return nullptr;
    
    return &iter->second;
}

vector<_float>* CAnimationManager::Find_AnimationSpeeds(const _wstring& strMacroTag)
{
    auto iter = m_AnimationSpeeds.find(strMacroTag);

    if (iter == m_AnimationSpeeds.end())
        return nullptr;

    return &iter->second;
}

vector<uint32_t>* CAnimationManager::Find_AnimationLoopTime(const _wstring& strMacroTag)
{
    auto iter = m_AnimationLoopTime.find(strMacroTag);

    if (iter == m_AnimationLoopTime.end())
        return nullptr;

    return &iter->second;
}

CAnimationManager* CAnimationManager::Create()
{
    CAnimationManager* pInstance = new CAnimationManager;
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ErrCode : CAnimationManager Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return  pInstance;
}

void CAnimationManager::Free()
{
    for (auto& pair : m_Animations)
        Safe_Release(pair.second);
    m_Animations.clear();

	CBase::Free();
}
