#include "Font.h"

CFont::CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont::Initialize(const _wstring& strFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, strFontFilePath.c_str());
	return S_OK;
}

HRESULT CFont::Draw_Text(SpriteBatch* pBatch, const _wstring& strText, const XMFLOAT2& vPos, FXMVECTOR vColor)
{
	m_pFont->DrawString(pBatch, strText.c_str(), vPos, vColor);
	return S_OK;
}

const XMFLOAT4& CFont::Measure_Text(const _wstring& strText)
{
	XMStoreFloat4(&m_vSize, m_pFont->MeasureString(strText.c_str()));
	return m_vSize;
}

CFont* CFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strFontFilePath)
{
	CFont* pInstance = new CFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(strFontFilePath)))
	{
		MSG_BOX("ErrCode : CFont Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFont::Free()
{
	CBase::Free();

	Safe_Delete(m_pFont);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}