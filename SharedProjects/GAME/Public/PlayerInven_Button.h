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

class CPlayerInven_Button final : public CButton
{
public:
	typedef struct tagPlayerInvenButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		ITEM_TYPE eItemType = {};

		_wstring strLeftText = {};
		_wstring strRightText = {};


	}PLAYERINVENBUTTON_DESC;

private:
	CPlayerInven_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerInven_Button(const CPlayerInven_Button& rhs);
	virtual ~CPlayerInven_Button() = default;

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
	ITEM_TYPE m_eItemType = { ITEM_TYPE::END };

	_wstring m_strLeftText = {};
	_wstring m_strRightText = {};
	 
	XMFLOAT4 m_vLeftTextSize = {};
	XMFLOAT4 m_vRightTextSize = {};

public:
	static CPlayerInven_Button* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END