#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CRenderManager : public CBase
{
private:
	CRenderManager();
	virtual ~CRenderManager() = default;

public:
	HRESULT Add_RenderGroup(RENDER_GROUP _RenderGroup, CObject* _Object);

public:
	HRESULT Render();

private:
	vector<CObject*> vecRenderGroup[static_cast<uint32_t>(RENDER_GROUP::END)];

public:
	static CRenderManager* Create();
	virtual void Free()override;
};

NS_END