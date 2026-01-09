#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CSetUIisEnable_Strgy final : public IOnClick
{
private:
	CSetUIisEnable_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSetUIisEnable_Strgy() = default;

public:
	HRESULT Initialize( _bool is);

public:
	void OnClick() override;

private:
	_bool m_isEnable = {};

public:
	static CSetUIisEnable_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool is);
	virtual void Free()override;
};

NS_END