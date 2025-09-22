#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CObjectManager : public CBase
{
private:
	CObjectManager();
	virtual ~CObjectManager() = default;

public:
	HRESULT Add_Object(const uint32_t _ObjectID, CObject* _Object);

public:
	int32_t Update(const _float fDT);
	int32_t Late_Update(const _float fDT);

private:
	map<uint32_t , vector<CObject*>> m_mapObject;
	int32_t m_iFlag = {};

public:
	static CObjectManager* Create();
	virtual void Free() override;
};

NS_END