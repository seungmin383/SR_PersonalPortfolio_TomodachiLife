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
class CWindow_Button final : public CButton
{
public:
	typedef struct tagWindowButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		uint8_t iOutlineTextureIndex = {};

		XMFLOAT4 vTargetPos = {};

		_float fLerpTime = {};
		_float fTargetFov = {};

	}WINDOWBUTTON_DESC;

private:
	CWindow_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWindow_Button(const CWindow_Button& rhs);
	virtual ~CWindow_Button() = default;

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
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };

	CRectBufferCom* m_pRectBufferCom = { nullptr };

	IOnClick* m_pClicked_Strgy = { nullptr };
	IOnClick* m_pSelected_Strgy = { nullptr };
	IOnClick* m_pSelectedClick_Strgy = { nullptr };

	uint8_t	m_iTextureIndex = {};
	uint8_t	m_iOutlineTextureIndex = {};

	_bool m_isLived = { FALSE };

	XMFLOAT4 m_vTargetPos = {};
	_float m_fLerpTime = {};
	_float m_fTargetFov = {};
	_float m_fLerpTimer = {};

public:
	static CWindow_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END