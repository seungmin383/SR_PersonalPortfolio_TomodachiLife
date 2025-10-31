#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CFileManager final : public CBase
{
private:
	CFileManager();
	virtual ~CFileManager() = default;

	
public:
	CFileManager* Create(int a);
	virtual void Free()override;
};

NS_END