#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLight final: public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	HRESULT Initialize(const LIGHT_DESC& eLight);

public:
	const LIGHT_DESC* Get_LightDesc() const { return &m_eLightDesc;	}

private:
	LIGHT_DESC m_eLightDesc = {};

public:
	static CLight* Create(const LIGHT_DESC& eLight);
	virtual void Free()override;
};

NS_END
