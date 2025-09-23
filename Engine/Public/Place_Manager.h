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
	HRESULT Go_Place(uint32_t _NextPlaceID, CPlace* _NextPlace);
	HRESULT Go_Back();

public:
	int32_t Update(const _float fDT);

private:
	vector<CPlace*> m_StackPlace;
	uint32_t m_iCurPlaceID = {};
	uint32_t m_iPrePlaceID = {};

public:
	static CPlaceManager* Create();
	virtual void Free()override;
};

NS_END