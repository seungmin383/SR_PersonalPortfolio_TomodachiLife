#pragma once
#include "UI.h"

NS_BEGIN(Client)

class CButton abstract : public CUI
{
public:
	typedef struct tagButtonDesc : public UI_DESC
	{
		VIEWPORT_TYPE eViewport;

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
	_bool Compare_Position(const POINT& vMousePos);

protected:
	VIEWPORT_TYPE m_eViewport = { VIEWPORT_TYPE::END };
	D3D11_VIEWPORT m_CurViewport = {};
	uint32_t m_iNumViewport = { 1 };

	_bool m_isEverClicked = { FALSE };
	_bool m_isClick = { FALSE };

public:
	virtual void On_Click()PURE;
	virtual void Free()override;
};

NS_END