#include "Sound_Manager.h"


CSoundManager::CSoundManager()
{
}

HRESULT CSoundManager::Initialize()
{
    FMOD_RESULT fmodResult;

    fmodResult = FMOD_System_Create(&m_pSystem, FMOD_VERSION);
    if (FMOD_OK != fmodResult)
        return E_FAIL;

    fmodResult = FMOD_System_Init(m_pSystem, 128, FMOD_INIT_NORMAL, NULL);
    if (FMOD_OK != fmodResult)
        return E_FAIL;

    for (uint32_t i = 0; i < static_cast<uint32_t>(SOUND_CHANNEL::END); ++i)
        m_Channels.emplace(static_cast<SOUND_CHANNEL>(i), vector<FMOD_CHANNEL*>{});

    return S_OK;
}

void CSoundManager::Update()
{
    if (nullptr != m_pBGMChannel)
    {
        FMOD_BOOL isPlaying = FALSE;
        FMOD_RESULT fmodResult = FMOD_Channel_IsPlaying(m_pBGMChannel, &isPlaying);

        if (FMOD_OK != fmodResult || !isPlaying)
            m_pBGMChannel = nullptr;
    }

    /* 순서 섞여도 될 때 swap-pop 으로 제거 -> O(1) */
    for (auto& pair : m_Channels)
    {
        auto& vec = pair.second;

        for (uint32_t i = 0 ; i < static_cast<uint32_t>(vec.size());)
        {
            if (nullptr == vec[i])
            {
                vec[i] = vec.back();
                vec.pop_back();

                /* i 번째에 back 을 넣고 back을 지웠으니까 i 번째를 다시 검사해야됨 */
                continue;
            }

            FMOD_BOOL isPlaying = FALSE;
            FMOD_RESULT fmodResult = FMOD_Channel_IsPlaying(vec[i], &isPlaying);

            if (FMOD_OK != fmodResult || !isPlaying)
            {
                vec[i] = vec.back();
                vec.pop_back();
                continue;
            }

            ++i;
        }
    }

    FMOD_System_Update(m_pSystem);
}

/* createSound -> SFX, createStream -> BGM */
HRESULT CSoundManager::Load_Sound(const _wstring& SoundTag, const _string& SoundFilePath, _bool isBGM, _bool isLoop)
{
    /* 사운드가 이미 있다는 뜻 */
    if (m_Sounds.find(SoundTag) != m_Sounds.end())
        return E_FAIL;

    FMOD_MODE Mode = FMOD_DEFAULT;
    Mode |= (isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

    FMOD_SOUND* pSound = nullptr;
    FMOD_RESULT fmodResult;

    if (isBGM)
        fmodResult = FMOD_System_CreateStream(m_pSystem, SoundFilePath.c_str(), Mode, 0, &pSound);
    else
        fmodResult = FMOD_System_CreateSound(m_pSystem, SoundFilePath.c_str(), Mode, 0, &pSound);

    if (FMOD_OK != fmodResult || nullptr == pSound)
    {
        MSG_BOX("ErrCode : Create Sound Failed");
        return E_FAIL;
    }

    m_Sounds.emplace(SoundTag, pSound);

    return S_OK;
}




HRESULT CSoundManager::Begin_BGM(const _wstring& SoundTag, _float fVolume)
{  
    /* 사운드가 없다는 뜻 */
    FMOD_SOUND* pSound = Find_Sound(SoundTag);
    if (nullptr == pSound)
        return E_FAIL;

    FMOD_RESULT fmodResult;

    /* 만약 실행중인 BGM이 있다면 */
    if (nullptr != m_pBGMChannel)
    {
        FMOD_Channel_Stop(m_pBGMChannel);
        m_pBGMChannel = nullptr;
    }

    fmodResult = FMOD_System_PlaySound(m_pSystem, pSound, nullptr, FALSE, &m_pBGMChannel);
    if (FMOD_OK != fmodResult)
        return E_FAIL;

    if (fVolume < 0.f)
        fVolume = 0.f;
    else if (fVolume > 1.f)
        fVolume = 1.f;

    fmodResult = FMOD_Channel_SetVolume(m_pBGMChannel, fVolume);
    if (FMOD_OK != fmodResult)
        return E_FAIL;

    return S_OK;
}

HRESULT CSoundManager::Begin_Sound(const _wstring& SoundTag, SOUND_CHANNEL eChannel, _float fVolume)
{
    /* 사운드가 없다는 뜻 */
    FMOD_SOUND* pSound = Find_Sound(SoundTag);
    if (nullptr == pSound)
        return E_FAIL;

    FMOD_CHANNEL* pChannel = nullptr;
    FMOD_RESULT fmodResult;

    fmodResult = FMOD_System_PlaySound(m_pSystem, pSound, nullptr, FALSE, &pChannel);
    if (FMOD_OK != fmodResult)
        return E_FAIL;

    if (fVolume < 0.f)
        fVolume = 0.f;
    else if (fVolume > 1.f)
        fVolume = 1.f;

    FMOD_Channel_SetVolume(pChannel, fVolume);
    
    m_Channels[eChannel].push_back(pChannel);
    return S_OK;
}

HRESULT CSoundManager::Stop_Sound(SOUND_CHANNEL eChannel)
{
    auto iter = m_Channels.find(eChannel);
    if (iter == m_Channels.end())
        return S_OK;

    for (auto& pChannel : iter->second)
    {
        if (nullptr != pChannel)
        {
            FMOD_Channel_Stop(pChannel);
            pChannel = nullptr;
        }
    }

    iter->second.clear();
    return S_OK;
}

HRESULT CSoundManager::Stop_AllSound()
{
    if (nullptr != m_pBGMChannel)
    {
        FMOD_Channel_Stop(m_pBGMChannel);
        m_pBGMChannel = nullptr;
    }

    for (auto& pair : m_Channels)
    {
        if (pair.first == SOUND_CHANNEL::NONE_STOP_SFX)
            continue;

        for (auto& pInst : pair.second)
        {
            if (nullptr != pInst)
                FMOD_Channel_Stop(pInst);
        }
        pair.second.clear();
    }

    return S_OK;
}


FMOD_SOUND* CSoundManager::Find_Sound(const _wstring SoundTag)
{
    auto iter = m_Sounds.find(SoundTag);

    if (iter == m_Sounds.end())
    {
        MSG_BOX("ErrCode : No Sound");
        return nullptr;
    }

    return iter->second;
}

CSoundManager* CSoundManager::Create()
{
    CSoundManager* pInstance = new CSoundManager;
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ErrCode : CSoundManager Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return  pInstance;
}

void CSoundManager::Free()
{
    if (nullptr != m_pBGMChannel)
    {
        FMOD_Channel_Stop(m_pBGMChannel);
        m_pBGMChannel = nullptr;
    }

    for (auto& pair : m_Channels)
    {
        for (auto& pInst : pair.second)
        {
            if (nullptr != pInst)
                FMOD_Channel_Stop(pInst);
        }
        pair.second.clear();
    }
    m_Channels.clear();

    for (auto& pair : m_Sounds)
    {
        if (nullptr != pair.second)
            FMOD_Sound_Release(pair.second);
    }
    m_Sounds.clear();

    if (nullptr != m_pSystem)
    {
        FMOD_System_Close(m_pSystem);
        FMOD_System_Release(m_pSystem);
        m_pSystem = nullptr;
    }

    CBase::Free();

    CBase::Free();
}
