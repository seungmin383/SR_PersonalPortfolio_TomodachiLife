#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CLight;

class CLightManager final : public CBase
{
private:
	CLightManager();
	virtual ~CLightManager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(uint32_t iIndex);

public:
	HRESULT Add_Light(const LIGHT_DESC& eLight);

private:
	list<CLight*> m_Lights;

public:
	static CLightManager* Create();
	virtual void Free() override;
};

NS_END