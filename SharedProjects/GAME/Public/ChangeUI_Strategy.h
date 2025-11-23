#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CChangeUI_Strategy final : public IOnClick
{
private:
	CChangeUI_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CChangeUI_Strategy() = default;

public:
	HRESULT Initialize(PLACEID eCurPlaceID,
		const vector<_wstring>& DisableLayers,
		const vector<_wstring>& EnableLayers);

public:
	void OnClick() override;

private:
	PLACEID m_eCurPlaceID = {};
	vector<_wstring > m_DisableLayers;
	vector<_wstring> m_EnableLayers;

public:
	static CChangeUI_Strategy* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, 
		PLACEID eCurPlaceID,
		const vector<_wstring>& DisableLayers,
		const vector<_wstring>& EnableLayers);

	virtual void Free()override;
};

NS_END