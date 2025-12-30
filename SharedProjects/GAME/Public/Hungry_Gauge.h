#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CHungry_Gauge final : public CUI
{
public:
	typedef struct tagHungryGaugeDesc : public UI_DESC
	{
	}HUNGRY_GAUGE_DESC;

protected:
	CHungry_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHungry_Gauge(const CHungry_Gauge& rhs);
	virtual ~CHungry_Gauge() = default;

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
	uint32_t m_iCurFullness = {};
	uint32_t m_iPreFullness = {};

	_bool m_isLevelUp = {};
	_bool m_isStartGaugeUp = {};

	_float m_fCurTexCoord = {};
	_float m_fPreTexCoord = {};
	_float m_fStartTexCoord = {};

	_float m_fTimeAcc = {};
	_float m_fRatio = {};

public:
	static CHungry_Gauge* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END