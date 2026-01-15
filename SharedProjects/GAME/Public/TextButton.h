#pragma once
#include "Game_Defines.h"
#include "Button.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)
class CIconTexture;
class IOnClick;

class CTextButton final : public CButton
{
public:
	enum TEXTTYPE {
		FIRSTNAME,
		LASTNAME,
		NICKNAME,
		GENDER,
		BLOOD,
		COLOR,
		BIRTH_YEAR,
		BIRTH_DAY,
		ADULT,
		RELATION,
		MAKER,
	};
	typedef struct tagTextButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pClicked_Strgy_1 = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		TEXTTYPE eTextType = {};

	}TEXTBUTTON_DESC;

private:
	CTextButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTextButton(const CTextButton& rhs);
	virtual ~CTextButton() = default;

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

	IOnClick* m_pClicked_Strgy			= { nullptr };
	IOnClick* m_pClicked_Strgy_1		= { nullptr };
	IOnClick* m_pSelected_Strgy			= { nullptr };
	IOnClick* m_pSelectedClick_Strgy	= { nullptr };

	_wstring m_strText = {};
	TEXTTYPE m_eTextType = {};

	_wstring m_strShowText = { };
	XMFLOAT4 m_vFontSize = {};

	_wstring m_strFistName = {};
	_wstring m_strLastName = {};
	_wstring m_strNickName = {};

public:
	static CTextButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END	