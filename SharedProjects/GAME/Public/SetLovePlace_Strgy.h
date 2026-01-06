
#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetLovePlace_Strgy final : public IOnClick
{
private:
	CSetLovePlace_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetLovePlace_Strgy() = default;

public:
	HRESULT Initialize(uint32_t i);

public:
	void OnClick() override;

private:
	uint32_t m_iPlace = {};

public:
	static CSetLovePlace_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t i);
	virtual void Free()override;
};

NS_END