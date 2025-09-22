#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLayer;
class CObjectManager : public CBase
{
private:
	CObjectManager();
	virtual ~CObjectManager() = default;

public:
	int32_t Update_Priority(const _float fDT);
	int32_t Update(const _float fDT);
	void	Update_Late(const _float fDT);


private:
	map<uint32_t , vector<CLayer*>> m_mapLayer;
	int32_t m_iFlag = {};

public:
	static CObjectManager* Create();
	virtual void Free() override;
};

NS_END