#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CObject;
class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_Object_in_Layer(const uint32_t _ObjectID, CObject* _Object);

public:
	int32_t Update_Priority(const _float fDT);
	int32_t Update(const _float fDT);
	void Update_Late(const _float fDT);

private:
	map <uint32_t, vector<CObject*>>			m_mapObject;
	int32_t  m_iFlag = { };

public:
	virtual void Free() override;	

};

NS_END