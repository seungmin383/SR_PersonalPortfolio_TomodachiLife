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
class IRender_Strgy;

class CPlaceMarker_Button final : public CButton
{
public:
	enum CLICKSTRGY_TYPE
	{
		CLICKED = 0,
		SELECTED = 1,
		SELECTEDCLICK = 2,
		END
	};

	enum CLICK_EVENT_STATE
	{
		CLICK_EVENT_STATE_PRESS = 0,
		CLICK_EVENT_STATE_PRESSED = 1
	};

public:
	typedef struct tagPlaceMarkerButtonDesc : public CButton::BUTTON_DESC
	{
		IClick_Strgy* pClick_Strgy = { nullptr };
		IClick_Strgy* pSelect_Strgy = { nullptr };
		IClick_Strgy* pSelectedClick_Strgy = { nullptr };

		CLICK_EVENT_STATE		eClickState = {};

		uint32_t iTextureIndex;
		uint32_t iOutlineTextureIndex;

	}PLACEMARKER_DESC;

private:
	CPlaceMarker_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlaceMarker_Button() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void	Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

	void Calc_ButtonState();

	int32_t On_Click() override;
	int32_t On_Select()override;
	int32_t On_SelectedClick()override;

private:
	CShaderCom* m_pShader = { nullptr };
	CTextureCom* m_pTexture = { nullptr };
	CTextureCom* m_pOutlineTexture = { nullptr };
	CRectBufferCom* m_pRectBuffer = { nullptr };

	IClick_Strgy* m_pClick_Strgy[CLICKSTRGY_TYPE::END] = {nullptr};
	IRender_Strgy* m_pRender_Strgy = { nullptr };

private:
	CLICK_EVENT_STATE m_eClickEventState;

	int32_t m_iReturnFlag = {};

	_bool m_HasClickedEvent = { FALSE };
	_bool m_HasSelectedEvent = { FALSE };
	_bool m_HasSelectedClickEvent = { FALSE };

	_bool m_isUnlock = { FALSE };
	uint32_t m_iTextureIndex = {};
	uint32_t m_iOutlineTextureIndex = {};

public:
	static CPlaceMarker_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END