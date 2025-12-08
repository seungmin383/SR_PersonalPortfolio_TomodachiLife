#include "IUnlock.h"
#include "GameInstance.h"

IUnlock::IUnlock()
    :m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

void IUnlock::Free()
{
    CBase::Free();
    Safe_Release(m_pGameInstance);
}

/* 옷가게 해금 조건 */
_bool CClothStore_UnlockStrategy::isUnlock()
{
    return _bool();
}

/* 음식점 해금 조건 */
_bool CFoodStore_UnlockStrategy::isUnlock()
{
    if (m_pGameInstance->Get_DIKeyState(DIK_Z))
        return TRUE;

    return FALSE;
}
