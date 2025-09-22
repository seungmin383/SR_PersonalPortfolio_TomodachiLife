#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CPrototype final : public CBase
{
private:
	CPrototype();
	virtual ~CPrototype() = default;

public:
	HRESULT Add_Prototype(int32_t _ObjectID, CObject* _Origin);
	CObject* Get_Object_Clone(int32_t _ObjectID);

private:
	//원본 저장
	map<int32_t, CObject*>		m_mapOrigin;
public:
	virtual void Free() override;
};

NS_END