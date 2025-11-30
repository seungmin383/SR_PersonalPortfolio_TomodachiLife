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
	// 레이어에 오브젝트를 추가하는 함수
	HRESULT Add_Object(CObject* pObject);		// 그 오브젝트

public:
	int32_t Update_Priority(_float fDT);
	int32_t Update(_float fDT);
	void Update_Late(_float fDT);

private:
	list<CObject*> m_ObjectList;

	_bool m_isBreak = { FALSE };

public:
	static CLayer* Create();
	virtual void Free() override;	
};

NS_END