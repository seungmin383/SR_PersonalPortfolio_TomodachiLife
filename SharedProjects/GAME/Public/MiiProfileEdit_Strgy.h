#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CMiiProfileEdit_Strgy final : public IOnClick
{
public:
	enum TEXTTYPE	{FIRSTNAME, LASTNAME, NICKNAME, END};

private:
	CMiiProfileEdit_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMiiProfileEdit_Strgy() = default;

public:
	HRESULT Initialize(_wstring strLayerTag, TEXTTYPE eType);

public:
	void OnClick() override;

private:
	_wstring m_strLayerTag = {};
	TEXTTYPE m_eTextType = {};

public:
	static CMiiProfileEdit_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		_wstring strLayerTag, TEXTTYPE eType);
	virtual void Free()override;
};

NS_END