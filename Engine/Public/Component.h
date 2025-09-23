#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class CComponent abstract : public CObject
{
protected:
	CComponent();
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual void Free()override;
};

NS_END