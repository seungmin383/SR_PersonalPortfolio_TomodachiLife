#pragma once
#include "Game_Defines.h"
#include "Actor.h"

#define TEST 1

NS_BEGIN(Engine)

class CShaderCom;
class CModel;
class CSEModel;
NS_END

NS_BEGIN(Game)

class CIsland final : public CActor
{
public:
	typedef struct tagIslandDesc : public ACTOR_DESC
	{
		
	}ISLAND_DESC;

protected:
	CIsland(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIsland(const CIsland& rhs);
	virtual ~CIsland() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

	void Calc_Object_Place_Point();

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();
	
private:
	CShaderCom* m_pShaderCom = { nullptr };

#ifdef TEST
	CModel* m_pModel = { nullptr };
#else
	CSEModel* m_pseModel = { nullptr };
#endif // TEST

	vector<_float2> m_vObjectPlaceVertices;

public:
	static CIsland* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END