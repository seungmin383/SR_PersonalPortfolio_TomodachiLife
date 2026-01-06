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

		_bool isLive = {};
		_wstring strMiiNickName = {};
		uint32_t iMiiIndex = {};

		_wstring strCameraTag = {TEXT("UI_Camera")};

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

	_wstring m_strMiiNickName = {};
	uint32_t m_iMiiIndex = {};

	uint32_t m_iWindowIndex = {};


	_wstring m_strCametaTag = {};

private:
	NEEDS m_eCurMii_Needs = { NEEDS::END };

public:
	static CWindow_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END