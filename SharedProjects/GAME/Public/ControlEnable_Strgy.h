#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CControlEnable_Strgy final : public IOnClick
{
public:
	typedef struct tagControlEnableStrgyDesc
	{
		PLACEID ePlaceID = {};
		vector<_wstring> EnableLayers;
		vector<_wstring> DisableLayers;

		_wstring strLayerTag = {};
	}CONTROLENABLESTRGY_DESC;

private:
	CControlEnable_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CControlEnable_Strgy() = default;

public:
	HRESULT Initialize(const CONTROLENABLESTRGY_DESC& Desc);

public:
	void OnClick() override;

private:
	uint32_t m_iPlaceID = {};


	vector<_wstring> m_EnableLayers;
	vector<_wstring> m_DisableLayers;
	_wstring m_strLayerTag = {};
public:
	static CControlEnable_Strgy* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		const CONTROLENABLESTRGY_DESC& Desc);


	virtual void Free()override;
};

NS_END