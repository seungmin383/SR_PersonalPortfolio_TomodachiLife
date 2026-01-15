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

class CCityhallMenu_Button final : public CButton
{
public:
	typedef struct tagCityhallMenuButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		_wstring strText = {};

		_bool isSpecialMove = { FALSE };
		_float fTargetFadeTime = {};

		_wstring strSoundTag;

	}CITYHALLMENUBUTTON_DESC;

private:
	CCityhallMenu_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCityhallMenu_Button(const CCityhallMenu_Button& rhs);
	virtual ~CCityhallMenu_Button() = default;

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
	void Fade_Enterance(_float fDT);
	void Fade_Exit(_float fDT);

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

	IOnClick* m_pClicked_Strgy = { nullptr };
	IOnClick* m_pSelected_Strgy = { nullptr };
	IOnClick* m_pSelectedClick_Strgy = { nullptr };


private:
	_float m_fOriginX = {};
	_float m_fOriginY = {};

	_wstring	m_strText = {};
	XMFLOAT4	m_vFontSize = {};

	_bool		m_isSpecialMove = { FALSE };
	XMFLOAT3	m_vTargetPos = {};
	XMFLOAT3	m_vInitPos = {};

	_bool m_isEnable = { FALSE };

	_float		m_fFadeTimer = {};
	_float		m_fTargetFadeTime = {};

	_bool		m_isFin_Fade = { FALSE };

	_float		m_fRatio = {};

	_wstring m_strSoundTag = {};

public:
	static CCityhallMenu_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END