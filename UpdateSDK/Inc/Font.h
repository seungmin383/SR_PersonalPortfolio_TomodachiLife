#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CFont final : public CBase
{
private:
	CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFont() = default;

public:
	HRESULT Initialize(const _wstring& strFontFilePath);
	HRESULT Draw_Text(
		SpriteBatch* pBatch, 
		const _wstring& strText,
		const XMFLOAT2& vPos,
		FXMVECTOR vColor);

	const XMFLOAT4& Measure_Text(const _wstring& strText);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	SpriteFont* m_pFont = { nullptr };

	XMFLOAT4 m_vSize = {};

public:
	static CFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strFontFilePath);
	virtual void Free() override;
};

NS_END