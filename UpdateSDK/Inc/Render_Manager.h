#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CRenderManager : public CBase
{
private:
	CRenderManager();
	virtual ~CRenderManager() = default;



public:
	CRenderManager* Create();
	virtual void Free()override;
};

NS_END