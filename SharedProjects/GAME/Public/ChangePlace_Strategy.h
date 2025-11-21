#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CChangePlace_Strategy final : public IOnClick
{
private:
	CChangePlace_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CChangePlace_Strategy() = default;

public:
	HRESULT Initialize(PLACE_ID eNextPlaceID);

public:
	void OnClick() override;

private:
	PLACE_ID m_eNextPlaceID = {};

public:
	static CChangePlace_Strategy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,PLACE_ID eNextPlaceID);
	virtual void Free()override;
};

NS_END