#pragma once
#include "Game_Defines.h"
#include "Actor.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CModel;

NS_END
NS_BEGIN(Game)

class CMii final : public CActor 
{
public:
	typedef struct tagMiiDesc : public ACTOR_DESC
	{

	}MII_DESC;

protected:
	CMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMii(const CMii& rhs);
	virtual ~CMii() = default;

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
	CModel* m_pModelCom = { nullptr };

public:
	static CMii* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END