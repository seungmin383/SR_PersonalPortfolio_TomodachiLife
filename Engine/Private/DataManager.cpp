#include "DataManager.h"

CDataManager::CDataManager()
{
}

CDataManager* CDataManager::Create()
{
	return nullptr;
}

void CDataManager::Free()
{
	CBase::Free();
}
