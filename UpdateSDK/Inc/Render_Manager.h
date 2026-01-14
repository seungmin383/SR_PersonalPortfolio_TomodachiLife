#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CGameInstance;

class CRectBufferCom;
class CShaderCom;

class CRenderManager : public CBase
{
private:
	CRenderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderManager() = default;

public:
	HRESULT Add_RenderGroup(RENDER_GROUP eRenderGroup, CObject* pObject);

public:
	HRESULT Initialize();
	HRESULT Render_Top();
	HRESULT Render_Bottom();

private:
	/* TOP */
	HRESULT Render_Top_Priority();
	HRESULT Render_Top_NonBlend();

	HRESULT Render_Top_Light();

	HRESULT Render_Top_NonLight();

	HRESULT Render_Top_Blend();
	HRESULT Render_Top_UI();
	HRESULT Render_Top_UI_Blend();

	HRESULT Render_Top_Combined();

	/* BOTTOM */
	HRESULT Render_Bottom_Priority();
	HRESULT Render_Bottom_NonBlend();

	HRESULT Render_Bottom_Blend();
	HRESULT Render_Bottom_UI();
	HRESULT Render_Bottom_UI_Blend();

#ifdef _DEBUG
	HRESULT Render_Debug();

#endif 

private:
	list<CObject*> m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::END)];
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };

	CShaderCom* m_pShader = { nullptr };
	CRectBufferCom* m_pBuffer = { nullptr };

private:
	uint32_t m_iViewportWidth = {};
	uint32_t m_iViewportHeight = {};

	XMFLOAT4X4 m_WorldMatrix = {};
	XMFLOAT4X4 m_ViewMatrix = {};
	XMFLOAT4X4 m_ProjMatrix = {};

public:
	static CRenderManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END