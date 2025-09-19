#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CObjectManager : public CBase
{
private:
	CObjectManager();
	virtual ~CObjectManager() = default;


private:
	vector<CObject*> m_vecObject[];

public:
	static CObjectManager* Create();
	virtual void Free() override;
};

NS_END