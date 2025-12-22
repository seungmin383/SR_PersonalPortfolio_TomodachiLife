#include "RenderTarget.h"
#include "GameInstance.h"
#include "RectBufferCom.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Initialize(const XMUINT2& vSize,	DXGI_FORMAT ePixelFormat, const XMFLOAT4& vClearColor)
{
	m_vClearColor = vClearColor;

	/* Texture2D */
	{
		D3D11_TEXTURE2D_DESC Desc = {};
		Desc.Width = vSize.x;
		Desc.Height = vSize.y;
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.Format = ePixelFormat;

		Desc.SampleDesc.Quality = 0;
		Desc.SampleDesc.Count = 1;

		Desc.Usage = D3D11_USAGE_DEFAULT;
		Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;

		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = 0;

		if (FAILED(m_pDevice->CreateTexture2D(&Desc, nullptr, &m_pTexture2D)))
			return E_FAIL;	

		/* RTV */
		if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
			return E_FAIL;
		/* SRV */
		if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
			return E_FAIL;
	}
	return S_OK;	
}

HRESULT CRenderTarget::Bind_ShaderResource(CShaderCom* pShader, const _char* pConstantName)
{
	return pShader->Bind_ShaderResource(pConstantName, m_pSRV);
}

void CRenderTarget::Clear_RenderTargetView()
{
	m_pContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<FLOAT*>(&m_vClearColor));
}

void CRenderTarget::Copy_Resource(ID3D11Resource* pDst)
{
	if (nullptr == pDst)
		return;

	m_pContext->CopyResource(pDst, m_pTexture2D);
}

#ifdef _DEBUG

HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fCX, _float fCY)
{
	uint32_t iNumViewport = { 1 };
	D3D11_VIEWPORT Desc = {};

	m_pContext->RSGetViewports(&iNumViewport, &Desc);

	XMStoreFloat4x4(&m_WorldMatrix,
		XMMatrixScaling(fCX, fCY, 1.f) *
		XMMatrixTranslation(fX - (Desc.Width * 0.5f), -fY + (Desc.Height * 0.5f), 0.f));

	return S_OK;
}

HRESULT CRenderTarget::Render(CShaderCom* pShader, CRectBufferCom* pBuffer)
{
	if(FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if(FAILED(pShader->Bind_ShaderResource("g_Texture", m_pSRV)))
		return E_FAIL;
	if(FAILED(pShader->Begin(0)))
		return E_FAIL;
	if(FAILED(pBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK	;
}

#endif 

CRenderTarget* CRenderTarget::Create(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const XMUINT2& vSize, DXGI_FORMAT ePixelFormat, const XMFLOAT4& vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);
	if (FAILED(pInstance->Initialize(vSize, ePixelFormat, vClearColor)))
	{
		MSG_BOX("ErrCode : CRenderTarget Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CRenderTarget::Free()
{
	CBase::Free();

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
