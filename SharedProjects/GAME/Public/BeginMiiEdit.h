#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CBeginMiiEdit_Strgy final : public IOnClick
{
private:
	CBeginMiiEdit_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBeginMiiEdit_Strgy() = default;

public:
	HRESULT Initialize(_bool isMan);

public:
	void OnClick() override;

private:
	_bool m_isMan = { FALSE };

public:
	static CBeginMiiEdit_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		_bool isMan);
	virtual void Free()override;
};

NS_END