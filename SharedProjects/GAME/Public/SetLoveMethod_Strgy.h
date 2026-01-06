#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetLoveMethod_Strgy final : public IOnClick
{
private:
	CSetLoveMethod_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetLoveMethod_Strgy() = default;

public:
	HRESULT Initialize(uint32_t i);

public:
	void OnClick() override;

private:
	uint32_t m_iMethod = {};

public:
	static CSetLoveMethod_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t i);
	virtual void Free()override;
};

NS_END