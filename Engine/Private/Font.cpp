#include "Font.h"

CFont::CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont::Initialize(const _tchar* pFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);
	return S_OK;
}

HRESULT CFont::Draw_Text(SpriteBatch* pBatch, const _tchar* pText, const XMFLOAT2& vPos, FXMVECTOR vColor)
{
	m_pFont->DrawString(pBatch, pText, vPos, vColor);
	return S_OK;
}

CFont* CFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CFont* pInstance = new CFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
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