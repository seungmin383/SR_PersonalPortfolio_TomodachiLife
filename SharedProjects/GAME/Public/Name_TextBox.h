#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CName_TextBox final : public CUI
{
public:
	typedef struct tagNameTextBoxDesc : public UI_DESC
	{
		/* 몇호인지 */
		uint32_t iRoomIndex = {};
		_wstring strName = {};

	}NAMETEXTBOX_DESC;

protected:
	CName_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CName_TextBox(const CName_TextBox& rhs);
	virtual ~CName_TextBox() = default;

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
	_wstring m_strNameText = {};
	_wstring m_strRoomText = {};

	XMFLOAT4 m_vColor = {};
	XMFLOAT4 m_vNameFontSize = {};
	XMFLOAT4 m_vRoomFontSize = {};

public:
	static CName_TextBox* Create(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END