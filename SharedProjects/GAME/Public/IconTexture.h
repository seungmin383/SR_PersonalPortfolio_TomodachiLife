#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CIconTexture final : public CUI
{
public:
	typedef struct tagIconTextureDesc : public UI_DESC
	{
		uint32_t iTextureIndex = {};

		_float TargetFadeTime;
		_bool isSpecialMove = { FALSE };

	}ICONTEXTURE_DESC;

protected:
	CIconTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIconTexture(const CIconTexture& rhs);
	virtual ~CIconTexture() = default;

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
	void Fade_Enterance(_float fDT);
	void Fade_Exit(_float fDT);

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

private:
	uint32_t m_iTextureIndex = {};


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


public:
	static CIconTexture* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END