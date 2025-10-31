#include "FileManager.h"

CFileManager::CFileManager()
{
}

CFileManager* CFileManager::Create()
{
	return new CFileManager;
}

void CFileManager::Free()
{
	CBase::Free();
}
