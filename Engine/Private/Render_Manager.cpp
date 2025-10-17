#include "Render_Manager.h"
#include "Object.h"
CRenderManager::CRenderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CRenderManager::Initialize()
{
	return S_OK;
}

HRESULT CRenderManager::Render()
{
	Render_Priority();
	Render_NonBlend();
	Render_Blend();
	Render_UI();
	return S_OK;
}

HRESULT CRenderManager::Render_Priority()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::PRIORITY)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Priority Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::PRIORITY)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_NonBlend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::NONBLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Non Blend Object Render Failed");
			return E_FAIL;
		}

		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::NONBLEND)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_Blend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode :Blend Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}
	
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BLEND)].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_UI()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::UI)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode : UI Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}

	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::UI)].clear();

	return S_OK;
}

HRESULT CRenderManager::Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object)
{
	if (nullptr == _Object)
		return E_FAIL;

	m_RenderGroup[static_cast<uint32_t>(_RenderGroup)].push_back(_Object);
	Safe_AddRef(_Object);

	return S_OK;
}

CRenderManager* CRenderManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderManager* pRenderManager = new CRenderManager(pDevice, pContext);

	if (FAILED(pRenderManager->Initialize()))
	{
		MSG_BOX("Failed to Created : RenderManager");
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

	CBase::Free();
}

