#include "Render_Manager.h"
#include "Object.h"
CRenderManager::CRenderManager()
{
}

CRenderManager* CRenderManager::Create()
{
	return new CRenderManager;
}

HRESULT CRenderManager::Render()
{
	for (int i = 0; i < static_cast<int32_t>(RENDER_GROUP::END); i++)
	{
		for (auto& Object : vecRenderGroup[i])
		{
			if (nullptr == Object)
				break;

			if (FAILED(Object->Render()))
			{
				MSG_BOX("ErrCode : Object Render Failed");
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT CRenderManager::Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object)
{
	if (nullptr == _Object)
		return E_FAIL;

	return S_OK;
}

void CRenderManager::Free()
{
	CBase::Free();
}

