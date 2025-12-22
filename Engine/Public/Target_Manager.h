#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CRenderTarget;

class CShaderCom;
class CRectBufferCom;

class CTargetManager final : public CBase
{
private:
	CTargetManager(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	virtual ~CTargetManager() = default;

public:
	HRESULT Add_RenderTarget(
		const _wstring& strTargetTag,
		const XMUINT2& vSize,
		DXGI_FORMAT ePixelFormat,
		const XMFLOAT4& vClearColor);

	HRESULT Add_MRT(
		const _wstring& strMRTTag, 
		const _wstring& strTargetTag);

	HRESULT Begin_MRT(const _wstring& strMRTTag, 
		ID3D11DepthStencilView* pDSV);
	HRESULT End_MRT();
	HRESULT Bind_ShaderResource(const _wstring& strTargetTag, 
		CShaderCom* pShader, const _char* pConstantName);

	void Copy_Resource(const _wstring& strTargetTag, ID3D11Resource* pDst);

#ifdef _DEBUG
	HRESULT Ready_Debug(const _wstring& strTargetTag,
		_float fX, _float fY, _float fCX, _float fCY);
	HRESULT Render(const _wstring& strMRTTag, 
		CShaderCom* pShader, CRectBufferCom* pBuffer);
#endif

private:
	CRenderTarget* Find_RenderTarget(const _wstring& strTargetTag);
	list<CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	ID3D11RenderTargetView* m_pBackBuffer = { nullptr };
	ID3D11DepthStencilView* m_pDSV = { nullptr };

private:
	map<const _wstring, CRenderTarget*> m_RTs;
	map<const _wstring, list<CRenderTarget*>> m_MRTs;

public:
	static CTargetManager* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	virtual void Free()override;
};

NS_END