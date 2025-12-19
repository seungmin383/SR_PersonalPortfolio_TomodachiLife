#include "Render_Manager.h"
#include "Object.h"
#include "GameInstance.h"
#include "UI.h"

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
	uint32_t iNumViewport = {1};
	D3D11_VIEWPORT Desc = {};

	m_pContext->RSGetViewports(&iNumViewport, &Desc);

	m_fViewportWidth = Desc.Width;
	m_fViewportHeight = Desc.Height;

	/* Render Targets */
	{
		// Target_Diffuse
		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Diffuse"), m_fViewportWidth, m_fViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, static_cast<XMFLOAT4>(Colors::Black))))
			return E_FAIL;
		// Target_Normal
		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Normal"), Desc.Width, Desc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, static_cast<XMFLOAT4>(Colors::Black))))
			return E_FAIL;
		// Target_Depth
		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Depth"), Desc.Width, Desc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, static_cast<XMFLOAT4>(Colors::Black))))
			return E_FAIL;
	}
	
	/* Object Multi Render Target */
	{
		_wstring strMRTTag = TEXT("MRT_GameObject");

		vector<_wstring> strTargetTagArr = {
			TEXT("Target_Diffuse"),
			TEXT("Target_Normal"),
			TEXT("Target_Depth"),
		};

		for (uint32_t i = 0; i < strTargetTagArr.size(); ++i)
		{
			if (FAILED(m_pGameInstance->TargetManager_Add_MRT(strMRTTag, strTargetTagArr[i])))
				return E_FAIL;
		}
	}

	/* Set Matrix */
	{
		XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(Desc.Width, Desc.Height, 1.f));
		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Desc.Width, Desc.Height, 0.f, 1.f));
	}
	

	return S_OK;
}

HRESULT CRenderManager::Render_Top()
{
	Render_Top_Priority();
	Render_Top_NonBlend();
	Render_Top_Blend();
	Render_Top_UI();
	Render_Top_UI_Blend();
	return S_OK;
}
HRESULT CRenderManager::Render_Bottom()
{
	Render_Bottom_Priority();
	Render_Bottom_NonBlend();
	Render_Bottom_Blend();
	Render_Bottom_UI();
	Render_Bottom_UI_Blend();
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
			MSG_BOX("ErrCode : Top Priority Object Render Failed");
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
			MSG_BOX("ErrCode : Top Non Blend Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_NONBLEND)].clear();
	return S_OK;
}

HRESULT CRenderManager::Render_Top_Blend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_BLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode : Top Blend Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_BLEND)].clear();
	return S_OK;
}

HRESULT CRenderManager::Render_Top_UI()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_UI)])
	{
		if (nullptr == Object)
			break;

		if (static_cast<CUI*>(Object)->Get_Visible())
		{
			if (FAILED(Object->Render()))
			{
				MSG_BOX("ErrCode : Top UI Object Render Failed");
				return E_FAIL;
			}
		}

		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_UI)].clear();
	return S_OK;
}

HRESULT CRenderManager::Render_Top_UI_Blend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_UI_BLEND)])
	{
		if (nullptr == Object)
			break;

		if (static_cast<CUI*>(Object)->Get_Visible())
		{
			if (FAILED(Object->Render()))
			{
				MSG_BOX("ErrCode : Top UI Blend Object Render Failed");
				return E_FAIL;
			}
		}

		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_UI_BLEND)].clear();
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
			MSG_BOX("ErrCode : Bottom Priority Object Render Failed");
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
			MSG_BOX("ErrCode : Bottom Non Blend Object Render Failed");
			return E_FAIL;
		}

		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_NONBLEND)].clear();
	return S_OK;
}

HRESULT CRenderManager::Render_Bottom_Blend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_BLEND)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode : Bottom Blend Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_BLEND)].clear();
	return S_OK;
}

HRESULT CRenderManager::Render_Bottom_UI()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_UI)])
	{
		if (nullptr == Object)
			break;

		if (static_cast<CUI*>(Object)->Get_Visible())
		{
			if (FAILED(Object->Render()))
			{
				MSG_BOX("ErrCode : Bottom UI Object Render Failed");
				return E_FAIL;
			}
		}
		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_UI)].clear();
	return S_OK;
}

HRESULT CRenderManager::Render_Bottom_UI_Blend()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_UI_BLEND)])
	{
		if (nullptr == Object)
			break;

		if (static_cast<CUI*>(Object)->Get_Visible())
		{
			if (FAILED(Object->Render()))
			{
				MSG_BOX("ErrCode : Bottom UI Blend Object Render Failed");
				return E_FAIL;
			}
		}
		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::BOTTOM_UI_BLEND)].clear();
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

