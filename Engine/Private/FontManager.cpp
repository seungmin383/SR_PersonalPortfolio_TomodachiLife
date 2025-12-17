#include "FontManager.h"
#include "Font.h"

CFontManager::CFontManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CFontManager::Initialize()
{
    m_pBatch = new SpriteBatch(m_pContext);
	return S_OK;
}

HRESULT CFontManager::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
    if (nullptr != Find_Font(strFontTag))
        return E_FAIL;

    CFont* pFont = CFont::Create(m_pDevice, m_pContext, pFontFilePath);
    if (nullptr == pFont)
        return E_FAIL;

    m_Fonts.emplace(strFontTag, pFont);

    return S_OK;
}

HRESULT CFontManager::Draw_Text(const _wstring& strFontTag, const _tchar* pText, const _float2& vPos, FXMVECTOR vColor)
{
    CFont* pFont = Find_Font(strFontTag);
    if (nullptr == pFont)
        return E_FAIL;  

    m_pContext->GSSetShader(nullptr, nullptr, 0);

    m_pBatch->Begin();

    if (FAILED(pFont->Draw_Text(m_pBatch, pText, vPos, vColor)))
        return E_FAIL;  
    m_pBatch->End();

	return S_OK;
}

CFont* CFontManager::Find_Font(const _wstring& strFontTag)
{
    auto    iter = m_Fonts.find(strFontTag);

    if (iter == m_Fonts.end())
        return nullptr;

    return iter->second;
}

CFontManager* CFontManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFontManager* pInstance = new CFontManager(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ErrCode : CFontManager Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CFontManager::Free()
{
    for (auto& Pair : m_Fonts)
        Safe_Release(Pair.second);
    m_Fonts.clear();

    Safe_Delete(m_pBatch);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

	CBase::Free();
}
