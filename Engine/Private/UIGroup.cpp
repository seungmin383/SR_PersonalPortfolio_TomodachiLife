#include "UIGroup.h"
#include "UI.h"

CUIGroup::CUIGroup()
{
}

void CUIGroup::UI_Update_Priority(_float fDT)
{
    if (m_UI.empty())
        return;

    for (auto& pair : m_UI) {
        pair.second->Update_Priority(fDT);
    }
}

void CUIGroup::UI_Update(_float fDT)
{
    if (m_UI.empty())
        return;

    for (auto& pair : m_UI) {
        pair.second->Update(fDT);
    }
}

void CUIGroup::UI_Update_Late(_float fDT)
{
    if (m_UI.empty())
        return;

    for (auto& pair : m_UI) {
        pair.second->Update_Late(fDT);
    }
}

HRESULT CUIGroup::UI_Render()
{
    if (m_UI.empty())
        return E_FAIL;

    for (auto& pair : m_UI) {
        if (FAILED(pair.second->Render()))
        {
            MSG_BOX("ErrCode : UI Render Failed");
            return E_FAIL;
        }
    }
    return S_OK;
}

HRESULT CUIGroup::Add_UI(const _wstring& strUITag, CUI* pUI)
{
    auto iter = m_UI.find(strUITag);
    if (m_UI.end() != iter)
    {
        MSG_BOX("ErrCode : UI Already Added");
        return E_FAIL;
    }

    Safe_AddRef(pUI);
    m_UI.emplace(strUITag, pUI);
    return S_OK;
}

void CUIGroup::Free()
{
    for (auto& pair : m_UI)
    {
        Safe_Release(pair.second);
    }
    m_UI.clear();
    CBase::Free();
}
