#pragma once
#include "Base.h"

NS_BEGIN(Engine)


class CDataManager final : public CBase
{
private:
	CDataManager();
	virtual ~CDataManager() = default;


public:

public :
	static CDataManager* Create();
	virtual void Free()override;
};

NS_END