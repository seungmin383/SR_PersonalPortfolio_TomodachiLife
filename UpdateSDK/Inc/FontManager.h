#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CFont;
class CFontManager final : public CBase
{
private:
	CFontManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFontManager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_Font(
		const _wstring& strFontTag, 
		const _wstring& strFontFilePath);

	HRESULT Draw_Text(
		const _wstring& strFontTag, 
		const _wstring& pText, 
		const _float2& vPos, 
		FXMVECTOR vColor);

	const XMFLOAT4& Measure_Text(
		const _wstring& strFontTag,
		const _wstring& pText);

private:
	CFont* Find_Font(const _wstring& strFontTag);

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	SpriteBatch*				m_pBatch = { nullptr };
	map<const _wstring, CFont*> m_Fonts;

public:
	static CFontManager* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END