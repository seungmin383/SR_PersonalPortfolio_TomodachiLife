#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CRenderManager : public CBase
{
private:
	CRenderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderManager() = default;

public:
	HRESULT Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object);

public:
	HRESULT Initialize();
	HRESULT Render();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

private:
	list<CObject*> m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::END)];
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	static CRenderManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END