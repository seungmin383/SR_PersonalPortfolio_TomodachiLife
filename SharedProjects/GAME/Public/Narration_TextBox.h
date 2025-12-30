#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CNarration_TextBox final : public CUI
{
public:
	typedef struct tagNarrationTextBoxDesc : public UI_DESC
	{
		_wstring strFontTag		= {};
		uint32_t iTextureIndex	= {};
		_wstring strText		= {};
		XMVECTORF32 vTextColor	= {};

	}NARRATION_TEXTBOX_DESC;

protected:
	CNarration_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNarration_TextBox(const CNarration_TextBox& rhs);
	virtual ~CNarration_TextBox() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

private:
	_wstring m_strFontTag = {};
	_wstring m_strText = {};
	uint32_t m_iTextureIndex = {};
	XMFLOAT4 m_vColor = {};

public:
	static CNarration_TextBox* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END