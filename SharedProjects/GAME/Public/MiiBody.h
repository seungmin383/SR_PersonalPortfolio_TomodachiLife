#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CSEModel;
class CShaderCom;

NS_END

NS_BEGIN(Game)

class CMiiBody final : public CPartObject
{
public:
	typedef struct tagMiiBodyDesc : public CPartObject::PARTOBJECT_DESC
	{

	}MIIBODY_DESC;

private:
	CMiiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiiBody(const CMiiBody& rhs);
	virtual ~CMiiBody() = default;

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
	CSEModel* m_pSeModel = { nullptr };
	CShaderCom* m_pShader = { nullptr };

public:
	static CMiiBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END