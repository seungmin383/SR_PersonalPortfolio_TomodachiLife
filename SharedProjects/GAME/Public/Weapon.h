#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CModel;
class CSEModel;
class CShaderCom;
NS_END

NS_BEGIN(Game)

class CWeapon final : public CPartObject
{
public:
	typedef struct tagWeaponDesc : public CPartObject::PARTOBJECT_DESC
	{
		const _int* pParentState = { nullptr };
		const _float4x4* pSocketMatrix = { nullptr };
	}WEAPON_DESC;

private:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs); 
	virtual ~CWeapon() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	CModel* m_pModel = { nullptr };
	CSEModel* m_pSeModel = { nullptr };
	CShaderCom* m_pShaderCom = { nullptr };

private:
	const int32_t* m_pParentState = { nullptr };
	const XMFLOAT4X4* m_pSocketMatrix = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CWeapon* Create(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END