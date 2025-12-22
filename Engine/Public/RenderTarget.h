#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CShaderCom;
class CRectBufferCom;

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	HRESULT Initialize(
		const XMUINT2& vSize,
		DXGI_FORMAT ePixelFormat,
		const XMFLOAT4& vClearColor);

	HRESULT Bind_ShaderResource(
		CShaderCom* pShader,
		const _char* pConstantName
	);

	void Clear_RenderTargetView();
	void Copy_Resource(ID3D11Resource* pDst);

#ifdef _DEBUG
	HRESULT Ready_Debug(
		_float fX, _float fY, 
		_float fCX, _float fCY);

	HRESULT Render(
		CShaderCom* pShader,
		CRectBufferCom* pBuffer);
#endif

public:
	ID3D11RenderTargetView* Get_RTV() const { return m_pRTV; }

private:
	ID3D11Device*				m_pDevice		= { nullptr };
	ID3D11DeviceContext*		m_pContext		= { nullptr };
	ID3D11Texture2D*			m_pTexture2D	= { nullptr };
	ID3D11RenderTargetView*		m_pRTV			= { nullptr };
	ID3D11ShaderResourceView*	m_pSRV			= { nullptr };

private:
	XMFLOAT4 m_vClearColor = {};

#ifdef _DEBUG
private:
	XMFLOAT4X4 m_WorldMatrix = {};

#endif

public:
	static CRenderTarget* Create(
		ID3D11Device*			pDevice, 
		ID3D11DeviceContext*	pContext,
		const XMUINT2&			vSize,
		DXGI_FORMAT				ePixelFormat,
		const XMFLOAT4&			vClearColor
	);

	virtual void Free()override;
};

NS_END