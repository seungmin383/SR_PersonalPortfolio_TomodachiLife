#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CTurnOffLight_Strgy final : public IOnClick
{
private:
	CTurnOffLight_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTurnOffLight_Strgy() = default;

public:
	HRESULT Initialize(_bool isMan);

public:
	void OnClick() override;

private:
	_bool m_isMan = { FALSE };

public:
	static CTurnOffLight_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		_bool isMan);
	virtual void Free()override;
};

NS_END