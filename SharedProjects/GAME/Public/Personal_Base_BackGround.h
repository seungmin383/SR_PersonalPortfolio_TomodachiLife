#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CPersonal_Base_BackGround final : public CUI
{
public:
	typedef struct tagPersonalBaseBackGroundDesc : public UI_DESC
	{
		_float TargetFadeTime;
	}PERSONALBASEBACKGROUND_DESC;

protected:
	CPersonal_Base_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPersonal_Base_BackGround(const CPersonal_Base_BackGround& rhs);
	virtual ~CPersonal_Base_BackGround() = default;

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

	void Fade_Movement(_float fDT);

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

private:
	_wstring m_strTitleText = {};
	_wstring m_strContentsText = {};

	XMFLOAT4 m_vTitleFontSize = {};
	XMFLOAT4 m_vContentsFontSize = {};

	uint32_t	m_iTextureIndex = {};

	_float m_fTimer = {};

	_bool tmp = { FALSE };

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
	static CPersonal_Base_BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END