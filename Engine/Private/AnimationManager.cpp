#include "AnimationManager.h"
#include "SEAnimation.h"

#define ANIM_BINARY_FOLDER_PATH L"../../SharedProjects/GAME/Asset/Animations"

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

    return S_OK;
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
