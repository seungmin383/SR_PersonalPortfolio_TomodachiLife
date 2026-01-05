#include "IUnlock.h"
#include "GameInstance.h"
#include "MiiManager.h"
#include "ApartManager.h"

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
    if (CApartManager::GetInstance()->Get_ApartInfo_NumMii() >= 2)
        return TRUE;

    return FALSE;
}

/* 음식점 해금 조건 */
_bool CFoodStore_UnlockStrategy::isUnlock()
{
    if (CApartManager::GetInstance()->Get_ApartInfo_NumMii() >= 2)
        return TRUE;

    return FALSE;
}

/* 아파트 */
_bool CApart_UnlockStrategy::isUnlock()
{
    return TRUE;
}

/* 시청 */
_bool CCityHall_UnlockStrategy::isUnlock()
{
    return TRUE;
}

/* 집가게 */
_bool CHouse_UnlockStrategy::isUnlock()
{
    if (CApartManager::GetInstance()->Get_ApartInfo_NumMii() >= 3)
        return TRUE;

    return FALSE;
}

/* 나머지 */
_bool COtherBuilding_UnlockStrategy::isUnlock()
{
    return FALSE;
}
