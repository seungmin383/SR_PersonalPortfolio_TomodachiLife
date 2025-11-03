#pragma once
#include "Game_Defines.h"
#include "Actor.h"

NS_BEGIN(Engine)

class CShaderCom;
class CModel;

NS_END

NS_BEGIN(Game)

class CSkyAndSea final : public CActor
{
public:
	typedef struct tagSkyAndSeaDesc : public ACTOR_DESC
	{

	}SKYANDSEA_DESC;
protected:
	CSkyAndSea(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkyAndSea(const CSkyAndSea& rhs);
	virtual ~CSkyAndSea() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void	Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CModel* m_pModel = { nullptr };

public:
	static CSkyAndSea* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END