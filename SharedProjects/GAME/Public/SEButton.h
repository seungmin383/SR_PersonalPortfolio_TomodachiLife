#pragma once
#include "Game_Defines.h"
#include "Button.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END


NS_BEGIN(Game)

class IClick_Strgy;
class ISelect_Strgy;
class IRender_Strgy;
class ISelectedClick_Strgy;

class CSEButton final : public CButton
{
public:
	enum CLICK_EVENT_STATE
	{
		CLICK_EVENT_STATE_PRESS = 0,
		CLICK_EVENT_STATE_PRESSED = 1
	};

	typedef struct tagButtonStateDesc
	{
		_bool hasSelectedEvent;
		CLICK_EVENT_STATE eClickEventState;
		CLICK_EVENT_STATE eSelectEventState;
	}BUTTONE_STATE_DESC;

public:
	typedef struct tagSEButtonDesc : public CButton::BUTTON_DESC
	{
		IClick_Strgy*			pClick_Strgy	= {nullptr};
		ISelect_Strgy*			pSelect_Strgy	= {nullptr};
		ISelectedClick_Strgy*	pSelectedClick_Strgy = { nullptr };
		IRender_Strgy*			pRender_Strgy	= {nullptr};
		int32_t					eButtonType = {};
		BUTTONE_STATE_DESC		ButtonStateDesc = {};

	}SEBUTTON_DESC;

private:
	CSEButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSEButton() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void	Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	void Calc_ButtonState();
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

	int32_t On_Click() override;
	int32_t On_Select()override;
	int32_t On_SelectedClick()override;
private:
	CShaderCom*				m_pShader = { nullptr };
	CTextureCom*			m_pTexture = { nullptr };
	CRectBufferCom*			m_pRectBuffer = { nullptr };

	IClick_Strgy*			m_pClick_Strgy = { nullptr };
	ISelect_Strgy*			m_pSelect_Strgy = { nullptr };
	ISelectedClick_Strgy*	m_pSelectedClick_Strgy = { nullptr };
	IRender_Strgy*			m_pRender_Strgy = { nullptr };

private:
	_bool m_hasSelectedEvent;
	CLICK_EVENT_STATE m_eClickEventState;
	CLICK_EVENT_STATE m_eSelectEventState;

	int32_t m_iReturnFlag = {};


public:
	static CSEButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END