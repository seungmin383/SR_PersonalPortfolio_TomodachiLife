#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CDeactive_Strgy final : public IOnClick
{

private:
	CDeactive_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CDeactive_Strgy() = default;

public:
	HRESULT Initialize(PLACEID ePlaceID, const _wstring& strLayerTag);

public:
	void OnClick() override;

private:
	PLACEID m_ePlaceID = {};
	_wstring m_strLayerTag = {};

public:
	static CDeactive_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PLACEID ePlaceID, const _wstring& strLayerTag);
	virtual void Free()override;
};

NS_END