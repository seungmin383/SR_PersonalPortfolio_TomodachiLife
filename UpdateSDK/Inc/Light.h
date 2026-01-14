#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CShaderCom;
class CRectBufferCom;
class CLight final: public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	void Render_Light(CShaderCom* pShader, CRectBufferCom* pBuffer);

public:
	const LIGHT_DESC* Get_LightDesc() const { return &m_LightDesc;	}

private:
	LIGHT_DESC m_LightDesc = {};

	_bool m_is = { FALSE };
	_bool m_Turn = { FALSE };

public:
	static CLight* Create(const LIGHT_DESC& LightDesc);
	virtual void Free()override;
};

NS_END
