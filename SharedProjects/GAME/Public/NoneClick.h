#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CNoneClick final : public IOnClick
{
private:
	CNoneClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNoneClick() = default;

public:
	HRESULT Initialize(_bool isMan);

public:
	void OnClick() override;

private:

public:
	static CNoneClick* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END