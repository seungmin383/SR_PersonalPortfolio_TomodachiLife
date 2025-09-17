#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CRenderManager : public CBase
{
private:
	CRenderManager();
	virtual ~CRenderManager() = default;

public:
	virtual void Render();

private:


public:
	static CRenderManager* Create();
	virtual void Free()override;
};

NS_END