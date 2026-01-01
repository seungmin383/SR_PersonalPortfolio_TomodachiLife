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
	_wstring m_strText = {};
	XMFLOAT4 m_vColor = {};
	XMFLOAT4 m_vFontSize = {};

public:
	static CNarration_TextBox* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END