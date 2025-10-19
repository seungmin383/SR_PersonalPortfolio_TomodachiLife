#pragma once
#include "Engine_Define.h"

NS_BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	virtual uint32_t AddRef();		//증가한 RefCnt를 반환
	virtual uint32_t Release();		//감소하기 전 RefCnt를 반환 -> 객체의 RefCnt가 감소했는지 / 객체가 삭제되었는지 확인 -> return 값이 0이면 삭제 / 1이면 감소

private:
	uint32_t	m_iRefCnt = {};

protected:
	virtual void Free();
};

NS_END