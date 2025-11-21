#include "Render_Manager.h"
#include "Object.h"
#include "GameInstance.h"

CRenderManager::CRenderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);

}

HRESULT CRenderManager::Add_RenderGroup(
	RENDER_GROUP eRenderGroup, 
	CObject* pObject)
{
	if (nullptr == pObject)
		return E_FAIL;

	m_RenderGroup[static_cast<uint32_t>(eRenderGroup)].push_back(pObject);
	Safe_AddRef(pObject);

	return S_OK;
}

HRESULT CRenderManager::Initialize()
{
	return S_OK;
}

HRESULT CRenderManager::Render_Top()
{
	Render_Top_Priority();
	Render_Top_NonBlend();
	Render_Top_Blend();
	Render_Top_UI();

	return S_OK;
}
HRESULT CRenderManager::Render_Bottom()
{
	Render_Bottom_Priority();
	Render_Bottom_NonBlend();
	Render_Bottom_Blend();
	Render_Bottom_UI();

	return S_OK;
}

HRESULT CRenderManager::Render_Top_Priority()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_PRIORITY)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Top Priority Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_PRIORITY)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_Top_NonBlend()
{

	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_NONBLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Top Non Blend Object Render Failed");
			return E_FAIL;
		}

		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_NONBLEND)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_Top_Blend()
{
	m_pGameInstance->GraphicDevice_Bind_RenderState(RENDER_STATE::NONE_BACKCULL);

	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_BLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Top Blend Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_BLEND)].clear();

	m_pGameInstance->GraphicDevice_Bind_RenderState(RENDER_STATE::BACKCULL);

	return S_OK;
}

HRESULT CRenderManager::Render_Top_UI()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_UI)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Top UI Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_UI)].clear();

	return S_OK;
}






HRESULT CRenderManager::Render_Bottom_Priority()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_PRIORITY)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Bottom Priority Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_PRIORITY)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_Bottom_NonBlend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_NONBLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Bottom Non Blend Object Render Failed");
			return E_FAIL;
		}

		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_NONBLEND)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_Bottom_Blend()
{
	m_pGameInstance->GraphicDevice_Bind_RenderState(RENDER_STATE::NONE_BACKCULL);
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_BLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Bottom Blend Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_BLEND)].clear();
	m_pGameInstance->GraphicDevice_Bind_RenderState(RENDER_STATE::BACKCULL);

	return S_OK;
}

HRESULT CRenderManager::Render_Bottom_UI()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_UI)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Bottom UI Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_UI)].clear();

	return S_OK;
}






CRenderManager* CRenderManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderManager* pRenderManager = new CRenderManager(pDevice, pContext);

	if (FAILED(pRenderManager->Initialize()))
	{
		MSG_BOX("Errcode : RenderManager Create Failed");
		Safe_Release(pRenderManager);
	}

	return pRenderManager;
}

void CRenderManager::Free()
{

	for (uint32_t i = 0; i < static_cast<uint32_t>(RENDER_GROUP::END); i++)
	{
		for (auto& Object : m_RenderGroup[i])
		{
			if (nullptr != Object)
				continue;

			Safe_Release(Object);
		}

		m_RenderGroup[i].clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

	CBase::Free();
}

