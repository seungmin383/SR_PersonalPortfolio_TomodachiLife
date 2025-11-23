#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CGameInstance;

class CRenderManager : public CBase
{
private:
	CRenderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderManager() = default;

public:
	HRESULT Add_RenderGroup(
		RENDER_GROUP eRenderGroup, 
		CObject* pObject);

public:
	HRESULT Initialize();
	HRESULT Render_Top();
	HRESULT Render_Bottom();

private:
	HRESULT Render_Top_Priority();
	HRESULT Render_Top_NonBlend();
	HRESULT Render_Top_Blend();
	HRESULT Render_Top_UI();
	HRESULT Render_Top_UI_Blend();

	HRESULT Render_Bottom_Priority();
	HRESULT Render_Bottom_NonBlend();
	HRESULT Render_Bottom_Blend();
	HRESULT Render_Bottom_UI();
	HRESULT Render_Bottom_UI_Blend();

private:
	list<CObject*> m_RenderGroup[static_cast<uint32_t>(RENDER_GROUP::END)];
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CRenderManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END