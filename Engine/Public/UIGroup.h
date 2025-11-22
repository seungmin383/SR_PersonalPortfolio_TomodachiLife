#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CUI;
class CUIGroup final : public CBase
{
private:
	CUIGroup();
	virtual ~CUIGroup() = default;

public:
	void UI_Update_Priority(_float fDT);
	void UI_Update(_float fDT);
	void UI_Update_Late(_float fDT);
	HRESULT UI_Render();

public:
	HRESULT Add_UI(const _wstring& strUITag, CUI* pUI);

public:
	map<_wstring, CUI*> m_UI;

public:
	static CUIGroup* Create() { return new CUIGroup; }
	virtual void Free()override;
};
NS_END