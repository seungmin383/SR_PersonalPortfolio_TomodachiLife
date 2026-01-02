#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetisBeFriend_Strgy final : public IOnClick
{
private:
	CSetisBeFriend_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetisBeFriend_Strgy() = default;

public:
	HRESULT Initialize(_bool is);

public:
	void OnClick() override;

private:
	_bool m_isBeFriend = {};

public:
	static CSetisBeFriend_Strgy* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		_bool is);
	virtual void Free()override;
};

NS_END