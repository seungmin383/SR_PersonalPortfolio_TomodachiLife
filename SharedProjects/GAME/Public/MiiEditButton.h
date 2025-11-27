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
class CMiiEditButton final : public CButton
{
public:
	typedef struct tagMiiEditButton : public BUTTON_DESC
	{
		IOnClick* pStrategy = { nullptr };

		uint32_t iTextureIndex = {};

		_wstring strLayerTag = {};

	}MIIEDITBUTTON_DESC;

private:
	CMiiEditButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiiEditButton(const CMiiEditButton& rhs);
	virtual ~CMiiEditButton() = default;

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

public:
	const uint32_t Get_iModelIndex()const { return m_iModelIndex; }
private:
	CShaderCom* m_pShader = { nullptr };
	CTextureCom* m_pTexture = { nullptr };
	CTextureCom* m_pIconTexture = { nullptr };
	CRectBufferCom* m_pRectBuffer = { nullptr };

	IOnClick* m_pClickStrategy = { nullptr };

private:
	_wstring m_strLayerTag = {};

	uint32_t m_iTextureIndex = {};
	uint32_t m_iModelIndex = {};

public:
	static CMiiEditButton* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END