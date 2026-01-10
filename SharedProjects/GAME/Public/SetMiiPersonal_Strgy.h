#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetMiiPersonal_Strgy final : public IOnClick
{
private:
	CSetMiiPersonal_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetMiiPersonal_Strgy() = default;

public:
	HRESULT Initialize(int32_t i);

public:
	void OnClick() override;

private:
	int32_t  m_iPersonal = {};

public:
	static CSetMiiPersonal_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int32_t i);
	virtual void Free()override;
};

NS_END