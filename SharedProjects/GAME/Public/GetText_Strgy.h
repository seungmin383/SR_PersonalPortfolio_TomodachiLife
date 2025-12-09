#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CGetText_Strgy final : public IOnClick
{
public:
	enum TEXTTYPE { FIRSTNAME, LASTNAME, NICKNAME, END };

private:
	CGetText_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CGetText_Strgy() = default;

public:
	HRESULT Initialize(_wstring strLayerTag, TEXTTYPE eType);

public:
	void OnClick() override;

private:
	_wstring m_strLayerTag = {};
	TEXTTYPE m_eTextType = {};

public:
	static CGetText_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END