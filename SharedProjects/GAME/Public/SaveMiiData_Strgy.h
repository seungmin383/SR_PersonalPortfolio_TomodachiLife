#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSaveMiiData_Strgy final : public IOnClick
{
private:
	CSaveMiiData_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSaveMiiData_Strgy() = default;

public:
	HRESULT Initialize();

public:
	void OnClick() override;

public:
	static CSaveMiiData_Strgy* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END