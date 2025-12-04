#pragma once
#include "UI.h"

NS_BEGIN(Engine)

class ENGINE_DLL CButton abstract : public CUI
{
public:
	typedef struct tagButtonDesc : public UI_DESC
	{

	}BUTTON_DESC;

protected:
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& rhs);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void	Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	void Compare_Position(const POINT& vMousePos);

public:
	void Set_isSelected(_bool is) { m_isSelected = is; }
	void Set_ButtonIndex(_int i) { m_iButtonIndex = i; }
	int32_t Get_ButtonIndex() { return m_iButtonIndex; }

protected:
	_bool m_isOverlappedMouse = { FALSE };
	_bool m_isClicked = { FALSE };
	_bool m_isSelected = { FALSE };

	_float2 m_fLeftTop = {};
	_float2 m_fRIghtBottom = {};

	_int m_iButtonIndex = 0;

public:
	virtual int32_t On_Click();
	virtual int32_t On_Select();
	virtual int32_t On_SelectedClick();

	virtual void Free()override;
};

NS_END