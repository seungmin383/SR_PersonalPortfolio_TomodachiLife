#include "AnimationManager.h"
#include "SEAnimation.h"

#define ANIM_BINARY_FOLDER_PATH L"../../SharedProjects/GAME/Asset/Animations"

#define ANIM_MACRO_DATA_FILE_PATH L"../../SharedProjects/GAME/GameData/AnimMacroData.json" 

CAnimationManager::CAnimationManager()
{
}

HRESULT CAnimationManager::Initialize()
{
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

        for (auto iter = j.begin(); iter != j.end(); ++iter)
        {
            const _string key = iter.key();
            const nlohmann::json& Arr = iter.value();

            vector<_wstring> macro;
            macro.reserve(Arr.size());

            for (const auto& value : Arr)
            {
                if (!value.is_string())
                    continue;

                macro.push_back(StrToWstr(value.get<_string>()));

            }

            m_AnimationMacros.emplace(StrToWstr(key), move(macro));
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
