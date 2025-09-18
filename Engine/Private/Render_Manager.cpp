#include "Render_Manager.h"

CRenderManager::CRenderManager()
{
}

CRenderManager* CRenderManager::Create()
{
	return new CRenderManager;
}

void CRenderManager::Free()
{
}

