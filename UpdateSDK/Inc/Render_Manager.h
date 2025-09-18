#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CRenderManager : public CBase
{
private:
	CRenderManager();
	virtual ~CRenderManager() = default;

public:
	HRESULT Render();

private:
	vector<uint32_t> vecRenderGroup[static_cast<uint32_t>(RENDER_GROUP::END)];

public:
	CRenderManager* Create();
	virtual void Free()override;
};

NS_END