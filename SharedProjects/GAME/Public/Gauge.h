#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CGauge final : public CUI
{
public:
	typedef struct tagGaugeDesc : public UI_DESC
	{
	}GAUGE_DESC;

protected:
	CGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGauge(const CGauge& rhs);
	virtual ~CGauge() = default;

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
	_float m_fCurLevel = {};
	_float m_fPreLevel = {};

	_bool m_isLevelUp = {};
	_bool m_isStartGaugeUp = {};

	_float m_fCurTexCoord = {};
	_float m_fPreTexCoord = {};
	_float m_fStartTexCoord = {};

	_float m_fTimeAcc = {};

public:
	static CGauge* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END