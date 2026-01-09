#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CEditOption_BackGround final : public CUI
{
public:
	typedef struct tagEditOptionBackGroundDesc : public UI_DESC
	{
		_float TargetFadeTime;

	}EDITOPTIONBACKGROUND_DESC;

protected:
	CEditOption_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEditOption_BackGround(const CEditOption_BackGround& rhs);
	virtual ~CEditOption_BackGround() = default;

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
	void Fade_Enterance(_float fDT	);
	void Fade_Exit(_float fDT);

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

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
	static CEditOption_BackGround* Create(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END