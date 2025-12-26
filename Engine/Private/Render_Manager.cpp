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
//	uint32_t iNumViewport = {1};
//	D3D11_VIEWPORT Desc = {};
//
//	m_pContext->RSGetViewports(&iNumViewport, &Desc);
//
//	m_iViewportWidth = static_cast<uint32_t>(Desc.Width);
//	m_iViewportHeight = static_cast<uint32_t>(Desc.Height);
//
//	/* Render Targets */
//	{
//		// Target_Diffuse
//		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Diffuse"), m_iViewportWidth, m_iViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f })))
//			return E_FAIL;
//		// Target_Normal
//		/* Normal은 범위가 고정된 방향벡터라 0 ~ 1 Unorm으로 저장해도 품질 손실이 거의 없고, 성능이 좋아서 사용함
//		대신 쉐이더에서 0 ~ 1로 보간해줘야 함*/
//		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Normal"), m_iViewportWidth, m_iViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f })))
//			return E_FAIL;
//		// Target_Depth
//		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Depth"), m_iViewportWidth, m_iViewportHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f })))
//			return E_FAIL;
//		// Target_Shade
//		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Shade"), m_iViewportWidth, m_iViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f })))
//			return E_FAIL;
//		/* Target_Specular */
//		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Specular"), m_iViewportWidth, m_iViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f })))
//			return E_FAIL;
//		// Target_Pick
//		if (FAILED(m_pGameInstance->TargetManager_Add_RenderTarget(TEXT("Target_Pick"), m_iViewportWidth, m_iViewportHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f })))
//			return E_FAIL;
//	}
//	
//	/* Object Multi Render Target */
//	{
//		_wstring strMRTTag = TEXT("MRT_GameObject");
//
//		vector<_wstring> strTargetTagArr = {
//			TEXT("Target_Diffuse"),
//			TEXT("Target_Normal"),
//			TEXT("Target_Depth"),
//			TEXT("Target_Pick"),
//		};
//
//		for (uint32_t i = 0; i < strTargetTagArr.size(); ++i)
//		{
//			if (FAILED(m_pGameInstance->TargetManager_Add_MRT(strMRTTag, strTargetTagArr[i])))
//				return E_FAIL;
//		}
//	}
//
//	/* Light Multi Render Target */
//	{
//		_wstring strMRTTag = TEXT("MRT_LightAcc");
//
//		vector<_wstring> strTargetTagArr = {
//			TEXT("Target_Shade"),
//			TEXT("Target_Specular"),
//		};
//
//		for (uint32_t i = 0; i < strTargetTagArr.size(); ++i)
//		{
//			if (FAILED(m_pGameInstance->TargetManager_Add_MRT(strMRTTag, strTargetTagArr[i])))
//				return E_FAIL;
//		}
//	}
//
//	/* Set Matrix */
//	{
//		XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(Desc.Width, Desc.Height, 1.f));
//		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
//		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Desc.Width, Desc.Height, 0.f, 1.f));
//	}
//	
//	/* Ready Component*/
//	{
//		m_pBuffer = CRectBufferCom::Create(m_pDevice, m_pContext);
//		if (nullptr == m_pBuffer)
//			return E_FAIL;
//
//		m_pShader = CShaderCom::Create(m_pDevice, m_pContext, TEXT("../../SharedProjects/GAME/Shader/Engine_Shader_Deferred.hlsl"), VTX_TEX::Elements, VTX_TEX::iNumElements);
//		if (nullptr == m_pShader)
//			return E_FAIL;
//
//	}
//
//#ifdef _DEBUG
//	if (FAILED(m_pGameInstance->TargetManager_Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f)))
//		return E_FAIL;
//	if (FAILED(m_pGameInstance->TargetManager_Ready_Debug(TEXT("Target_Normal"), 300.f, 100.f, 200.f, 200.f)))
//		return E_FAIL;
//	if (FAILED(m_pGameInstance->TargetManager_Ready_Debug(TEXT("Target_Depth"), 500.f, 100.f, 200.f, 200.f)))
//		return E_FAIL;
//	if (FAILED(m_pGameInstance->TargetManager_Ready_Debug(TEXT("Target_Shade"), 100.f, 300.f, 200.f, 200.f)))
//		return E_FAIL;
//	if (FAILED(m_pGameInstance->TargetManager_Ready_Debug(TEXT("Target_Specular"), 300.f, 300.f, 200.f, 200.f)))
//		return E_FAIL;
//
//
//#endif 

	return S_OK;
}

HRESULT CRenderManager::Render_Top()
{
	Render_Top_Priority();
	Render_Top_NonBlend();

	//Render_Top_Light();
	//Render_Top_Combined();

	Render_Top_NonLight();

	Render_Top_Blend();
	Render_Top_UI();
	Render_Top_UI_Blend();

	//Render_Debug();

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
	 //if (FAILED(m_pGameInstance->TargetManager_Begin_MRT(TEXT("MRT_GameObject"), nullptr)))
	 //	return E_FAIL;

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

	 //if (FAILED(m_pGameInstance->TargetManager_End_MRT()))
	 //	return E_FAIL;

	return S_OK;
}

HRESULT CRenderManager::Render_Top_Light()
{	
	if (FAILED(m_pGameInstance->TargetManager_Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix_Inv(m_pShader,"g_ViewMatrixInv", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix_Inv(m_pShader,"g_ProjMatrixInv", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShader, ("g_vCamPosition"), TEXT("World_Camera"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	m_pGameInstance->LightManager_Render_Light(m_pShader, m_pBuffer);

	if (FAILED(m_pGameInstance->TargetManager_End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderManager::Render_Top_Combined()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->TargetManager_Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
	//	return E_FAIL;

	if (FAILED(m_pShader->Begin(static_cast<uint32_t>(DEFERRED::COMBINED))))
		return E_FAIL;
	if (FAILED(m_pBuffer->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_pBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderManager::Render_Top_NonLight()
{
	for (auto& Object : m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_NONLIGHT)])
	{
		if (nullptr == Object)
			break;

		if (FAILED(Object->Render()))
		{
			MSG_BOX("ErrCode : TOP_NONLIGHT Object Render Failed");
			return E_FAIL;
		}
		Safe_Release(Object);
	}
	m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::TOP_NONLIGHT)].clear();
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

HRESULT CRenderManager::Render_Debug()
{
	m_pBuffer->Bind_Resource();
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pGameInstance->TargetManager_Render(TEXT("MRT_GameObject"), m_pShader, m_pBuffer);
	m_pGameInstance->TargetManager_Render(TEXT("MRT_LightAcc"), m_pShader, m_pBuffer);

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
			if (nullptr == Object)
				continue;

			Safe_Release(Object);
		}
		m_RenderGroup[i].clear();
	}

	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

	CBase::Free();
}

