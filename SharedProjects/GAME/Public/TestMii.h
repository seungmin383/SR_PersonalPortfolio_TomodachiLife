#pragma once
#include "Game_Defines.h"
#include "Actor.h"
#include "ConvertJson.h"

#define ANIM_TEST 0

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CModel;
class CSEModel;

NS_END

NS_BEGIN(Game)

class CTestMii final : public CActor 
{
public:
	typedef struct tagTestMiiDesc : public ACTOR_DESC
	{

	}TEST_MII_DESC;

protected:
	CTestMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTestMii(const CTestMii& rhs);
	virtual ~CTestMii() = default;

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

	MII_INFO m_eInfo = {};

public:
	static CTestMii* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END