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

class CPersonal_Button final : public CButton
{
public:
	typedef struct tagPersonalButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		uint32_t iTextureIndex = {};

		XMFLOAT4 vFixColor;

	}PERSONALBUTTON_DESC;

private:
	CPersonal_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPersonal_Button(const CPersonal_Button& rhs);
	virtual ~CPersonal_Button() = default;

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

	XMFLOAT4 m_vFixColor = {};
	XMFLOAT4 m_vSelectedColor = {};
	XMFLOAT4 m_vToggleColor = {};

	_float m_fToggleTimer = {};
	_bool m_isToggle = {};
	_float m_fToggleHoldTimer = {};

public:
	static CPersonal_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END