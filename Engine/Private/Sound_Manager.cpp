#include "Sound_Manager.h"


CSoundManager::CSoundManager()
{
}

HRESULT CSoundManager::Initialize()
{
    FMOD_System_Create(&m_pSystem, FMOD_VERSION);
    FMOD_System_Init(m_pSystem, 128, FMOD_INIT_NORMAL, NULL);

    return S_OK;
}


void CSoundManager::Update()
{
}

/* createSound -> SFX, createStream -> BGM */
HRESULT CSoundManager::Load_Sound(const _wstring& SoundTag, const _wstring& SoundFilePath, _bool isBGM, _bool isLoop)
{
    return S_OK;
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
    CBase::Free();
}
