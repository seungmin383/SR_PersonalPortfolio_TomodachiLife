#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CBuyFood_Strgy final : public IOnClick
{
private:
	CBuyFood_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBuyFood_Strgy() = default;

public:
	HRESULT Initialize();

public:
	void OnClick() override;

public:
	static CBuyFood_Strgy* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END