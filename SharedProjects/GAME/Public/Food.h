#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CFood final : public CUI
{
protected:
	CFood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFood(const CFood& rhs);
	virtual ~CFood() = default;

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
	CShaderCom*		m_pShaderCom		= { nullptr };
	CTextureCom*	m_pTextureCom		= { nullptr };
	CRectBufferCom* m_pRectBufferCom	= { nullptr };

public:
	static CFood* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END