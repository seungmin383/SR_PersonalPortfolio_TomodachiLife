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

class CCommonButton final : public CButton
{
public:
	typedef struct tagCommonButton : public BUTTON_DESC
	{
		PLACEID eTexturePlaceID = {};
		PLACEID eIconTexturePlaceID = {};

		IOnClick* pStrategy = {nullptr};

		_wstring strTextureTag = {};

		uint32_t iTextureIndex = {};

	}COMMONBUTTON_DESC;

private:
	CCommonButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCommonButton(const CCommonButton& rhs);
	virtual ~CCommonButton() = default;

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

private:
	PLACEID m_eTexturePlaceID = {PLACEID::END};

	_wstring m_strTextureTag = {};

	uint32_t m_iTextureIndex = {};

public:
	static CCommonButton* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END