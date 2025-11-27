#include "MiiManager.h"

IMPLEMENT_SINGLETON(CMiiManager)

CMiiManager::CMiiManager()
{
}

void CMiiManager::Ready_EditMii(const MIIINFO_DESC& CurMiiDesc)
{
	m_CurMiiDesc = CurMiiDesc;
}

void CMiiManager::Free()
{
	CBase::Free();
}
