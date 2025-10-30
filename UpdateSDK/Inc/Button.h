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

protected:
	_bool m_isOverlappedMouse = { FALSE };
	_bool m_isClicked = { FALSE };

	_float2 m_fLeftTop = {};
	_float2 m_fRIghtBottom = {};

public:
	virtual void On_Click()PURE;
	virtual void Free()override;
};

NS_END