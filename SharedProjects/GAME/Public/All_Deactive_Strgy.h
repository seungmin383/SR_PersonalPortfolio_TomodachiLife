#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CAll_Deactive_Strgy final : public IOnClick
{

private:
	CAll_Deactive_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CAll_Deactive_Strgy() = default;

public:
	HRESULT Initialize(PLACEID ePlaceID);

public:
	void OnClick() override;

private:
	PLACEID m_ePlaceID = {};

public:
	static CAll_Deactive_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PLACEID ePlaceID);
	virtual void Free()override;
};

NS_END