#pragma once
#include "Game_Defines.h"
#include "Actor.h"

// #define LOADING_MODEL 0

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CModel;
class CSEModel;

NS_END

NS_BEGIN(Game)

class CStore_Food final : public CActor
{
public:
	typedef struct tagStoreFoodDesc : public ACTOR_DESC
	{

	}STORE_FOOD_DESC;

protected:
	CStore_Food(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStore_Food(const CStore_Food& rhs);
	virtual ~CStore_Food() = default;

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
	CSEModel* m_pSeModel = { nullptr };

public:
	static CStore_Food* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END