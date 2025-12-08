#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CChangePlace_Strgy final : public IOnClick
{
private:
	CChangePlace_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CChangePlace_Strgy() = default;

public:
	HRESULT Initialize(PLACEID eNextPlaceID, const _wstring& strLayerTag);

public:
	void OnClick() override;

private:
	PLACEID m_eNextPlaceID = {};
	_wstring m_strLayerTag = {};
public:
	static CChangePlace_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,PLACEID eNextPlaceID, const _wstring& strLayerTag);
	virtual void Free()override;
};

NS_END