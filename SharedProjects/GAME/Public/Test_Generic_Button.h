#pragma once
#include "Game_Defines.h"
#include "Button.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)
class IOnClick;

class CTest_Generic_Button final : public CButton
{
public:
	typedef struct tagPersonalButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		uint32_t iTextureIndex = {};

		XMFLOAT4 vFixColor;

		int32_t iPersonal = {};

	}PERSONALBUTTON_DESC;

private:
	CTest_Generic_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTest_Generic_Button(const CTest_Generic_Button& rhs);
	virtual ~CTest_Generic_Button() = default;

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

	int32_t On_Click() override;

private:
	CShaderCom* m_pShader = { nullptr };
	CTextureCom* m_pTexture = { nullptr };
	CRectBufferCom* m_pRectBuffer = { nullptr };

	IOnClick* m_pClicked_Strgy = { nullptr };

	IOnClick* m_pSelected_Strgy = { nullptr };
	IOnClick* m_pSelectedClick_Strgy = { nullptr };

private:
	XMFLOAT4 m_vFixColor = {};
	XMFLOAT4 m_vSelectedColor = {};
	XMFLOAT4 m_vToggleColor = {};

	_float m_fToggleTimer = {};
	_bool m_isToggle = {};
	_float m_fToggleHoldTimer = {};

	int32_t m_iPersonal = {};

	_bool m_isPreSelected = {};

public:
	static CTest_Generic_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};



struct UIParams
{
	_float fX = 0.f;
	_float fY = 0.f;
	_float fZ = 0.f;

	_float fCX = 0.f;
	_float fCY = 0.f;

	_float fAlpha = 1.f;
	_float fScaleX = 1.f;
	_float fScaleY = 1.f;
};

class IUITrigger
{
public:
	virtual ~IUITrigger() = default;
	virtual void Reset() = 0;
	virtual _bool Tick(_float fDT) = 0;
};

class IUITransition
{
public:
	virtual ~IUITransition() = default;
	virtual void Reset() = 0;
	virtual void Begin(const UIParams& from, const UIParams& to) = 0;
	virtual _bool Tick(_float fDT, UIParams& inout) = 0; // true: end, false: playing
};

class CGenericButton
{
public:
	enum class PHASE
	{
		HIDDEN,
		ENTERING,
		IDLE,
		EXITING,
	};

public:
	CGenericButton() = default;
	~CGenericButton() = default;

public:
	void Set_Base(const UIParams& base);
	void Set_Hidden(const UIParams& hidden);

	void Set_ShowTrigger(std::unique_ptr<IUITrigger> p);
	void Set_HideTrigger(std::unique_ptr<IUITrigger> p);

	void Set_Enter(std::unique_ptr<IUITransition> p);
	void Set_Exit(std::unique_ptr<IUITransition> p);

	void Set_OnClick(std::function<void(void)> fn);
	void Set_Render(std::function<void(const UIParams&)> fn);

	void Set_isInputEnabled(_bool is);

	void Show();
	void Hide();

	void Tick(_float fDT, _float fMouseX, _float fMouseY, _bool isLDownEdge, _bool isLUpEdge);
	void Render();

public:
	PHASE Get_Phase() const;
	const UIParams& Get_Cur() const;

private:
	_bool HitTest(_float mx, _float my) const;

	void Begin_Enter();
	void Begin_Exit();

private:
	UIParams m_Base = {};
	UIParams m_Hidden = {};
	UIParams m_Cur = {};

	std::unique_ptr<IUITrigger> m_pShowTrigger;
	std::unique_ptr<IUITrigger> m_pHideTrigger;

	std::unique_ptr<IUITransition> m_pEnter;
	std::unique_ptr<IUITransition> m_pExit;

	std::function<void(void)> m_fnOnClick;
	std::function<void(const UIParams&)> m_fnRender;

	PHASE m_ePhase = PHASE::HIDDEN;

	_bool m_isInputEnabled = false;
	_bool m_isPressed = false;
};
NS_END