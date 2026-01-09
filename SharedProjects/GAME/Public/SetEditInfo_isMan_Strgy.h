#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetEditInfo_isMan_Strgy final : public IOnClick
{
private:
	CSetEditInfo_isMan_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetEditInfo_isMan_Strgy() = default;

public:
	HRESULT Initialize(_bool is);

public:
	void OnClick() override;

private:
	_bool m_isMan = {};

public:
	static CSetEditInfo_isMan_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool is);
	virtual void Free()override;
};

NS_END