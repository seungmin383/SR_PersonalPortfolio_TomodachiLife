#include "Target_Manager.h"
#include "RenderTarget.h"

CTargetManager::CTargetManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTargetManager::Add_RenderTarget(const _wstring& strTargetTag,
	const XMUINT2& vSize, DXGI_FORMAT ePixelFormat, const XMFLOAT4& vClearColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget* pRT = CRenderTarget::Create(m_pDevice, m_pContext, vSize, ePixelFormat, vClearColor);
	
	if (nullptr == pRT)
		return E_FAIL;

	m_RTs.emplace(strTargetTag, pRT);

	return S_OK;
}

HRESULT CTargetManager::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	CRenderTarget* pRT = Find_RenderTarget(strTargetTag);
	if (nullptr == pRT)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>    MRTList;

		MRTList.push_back(pRT);

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRT);

	Safe_AddRef(pRT);
	return S_OK;
}

HRESULT CTargetManager::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	/* AddRef µÈ m_pBackBuffer, m_pDSV */
	m_pContext->OMGetRenderTargets(1, &m_pBackBuffer, &m_pDSV);

	ID3D11RenderTargetView* pRTVs[8] = { nullptr };
	uint32_t iNumRTV = {};

	for (CRenderTarget* pRT : *pMRTList)
	{
		pRT->Clear_RenderTargetView();
		pRTVs[iNumRTV++] = pRT->Get_RTV();
	}

	if (nullptr != pDSV)
	{
		m_pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		m_pContext->OMSetRenderTargets(iNumRTV, pRTVs, pDSV);
	}
	else
		m_pContext->OMSetRenderTargets(iNumRTV, pRTVs, m_pDSV);

	return S_OK;
}

HRESULT CTargetManager::End_MRT()
{
	ID3D11RenderTargetView* pRTVs[8] = {
		m_pBackBuffer
	};

	m_pContext->OMSetRenderTargets(8, pRTVs, m_pDSV);

	Safe_Release(m_pBackBuffer);
	Safe_Release(m_pDSV);

	return S_OK;
}

HRESULT CTargetManager::Bind_ShaderResource(const _wstring& strTargetTag, CShaderCom* pShader, const _char* pConstantName)
{
	CRenderTarget* pRT = Find_RenderTarget(strTargetTag);
	if (nullptr == pRT)
		return E_FAIL;

	return pRT->Bind_ShaderResource(pShader, pConstantName);
}

HRESULT CTargetManager::Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fCX, _float fCY)
{
	CRenderTarget* pRT = Find_RenderTarget(strTargetTag);
	if (nullptr == pRT)
		return E_FAIL;

	return pRT->Ready_Debug(fX, fY, fCX, fCY);
}

HRESULT CTargetManager::Render(const _wstring& strMRTTag, CShaderCom* pShader, CRectBufferCom* pBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRT : *pMRTList)
	{
		pRT->Render(pShader, pBuffer);
	}

	return S_OK;
}

CRenderTarget* CTargetManager::Find_RenderTarget(const _wstring& strTargetTag)
{
	auto    iter = m_RTs.find(strTargetTag);
	if (iter == m_RTs.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CTargetManager::Find_MRT(const _wstring& strMRTTag)
{
	auto    iter = m_MRTs.find(strMRTTag);
	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

CTargetManager* CTargetManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CTargetManager(pDevice, pContext);
}

void CTargetManager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RTs)
		Safe_Release(Pair.second);
	m_RTs.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	CBase::Free();
}
