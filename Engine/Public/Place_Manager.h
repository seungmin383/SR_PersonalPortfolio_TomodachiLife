#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CPlace;

class CPlaceManager :public CBase
{
private:
	CPlaceManager();
	virtual ~CPlaceManager() = default;

public:
	HRESULT Go_Place(CPlace* _CPlace);
	HRESULT Go_Back();

public:
	int32_t Update(const _float fDT);

private:
	stack<CPlace*> m_StackPlace;

public:
	static CPlaceManager* Create();
	virtual void Free()override;
};

NS_END