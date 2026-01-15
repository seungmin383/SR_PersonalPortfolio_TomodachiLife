#pragma once
#include "Game_Defines.h"
#include "Button.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)
class CItemManager;
class IOnClick;

class CCategory_Button final : public CButton
{
public:
	typedef struct tagCategoryButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pClicked_Strgy_1 = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };
		_wstring strText = {};

		_bool isSpecialMove = { FALSE };
		_float TargetFadeTime;

		_wstring strSoundTag = { L"" };

	}CATEGORYBUTTON_DESC;

private:
	CCategory_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCategory_Button(const CCategory_Button& rhs);
	virtual ~CCategory_Button() = default;

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
	CShaderCom* m_pShader = { nullptr };
	CTextureCom* m_pTexture = { nullptr };
	CRectBufferCom* m_pRectBuffer = { nullptr };

	IOnClick* m_pClicked_Strgy = { nullptr };
	IOnClick* m_pClicked_Strgy_1 = { nullptr };
	IOnClick* m_pSelected_Strgy = { nullptr };
	IOnClick* m_pSelectedClick_Strgy = { nullptr };

private:
	_bool m_isSpecialMove = { FALSE };

	_wstring m_strText = {};
	_wstring m_strTmpText = {};
	_wstring m_strShowText = {};


	XMFLOAT4 m_vFontSize = {};

	_wstring m_strSoundTag = {};


private:
	_float m_fOriginX = {};
	_float m_fOriginY = {};

	_bool m_isEnable = { FALSE };
	XMFLOAT3	m_vTargetPos = {};
	XMFLOAT3	m_vInitPos = {};

	_float m_fFadeTimer = {};
	_float m_fTargetFadeTime = {};

	_bool m_isFin_Fade = { FALSE };

	_float m_fRatio = {};

public:
	static CCategory_Button* Create(ID3D11Device* pDevice,	ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END