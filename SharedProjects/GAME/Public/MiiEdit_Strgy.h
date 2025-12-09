#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CMiiEdit_Strgy final : public IOnClick
{
public:
	enum PARTS { HEAD, HAIR, EYEBROW, EYES, NOSE, MOUTH, END };

private:
	CMiiEdit_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMiiEdit_Strgy() = default;

public:
	HRESULT Initialize(_wstring strLayerTag, PARTS ePart, uint32_t iIndex);

public:
	void OnClick() override;

private:
	_wstring m_strLayerTag = {};
	PARTS m_ePart = {};
	uint32_t m_iIndex = {};

public:
	static CMiiEdit_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		_wstring strLayerTag, PARTS ePart, uint32_t iIndex);
	virtual void Free()override;
};

NS_END