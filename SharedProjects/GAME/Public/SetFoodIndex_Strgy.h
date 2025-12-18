#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetFoodIndex_Strgy final : public IOnClick
{
private:
	CSetFoodIndex_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetFoodIndex_Strgy() = default;

public:
	HRESULT Initialize(uint32_t iIndex);

public:
	void OnClick() override;

private:
	uint32_t m_iIndex = {};

public:
	static CSetFoodIndex_Strgy* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		uint32_t iIndex);
	virtual void Free()override;
};

NS_END