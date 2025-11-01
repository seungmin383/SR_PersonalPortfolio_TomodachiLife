#pragma once
#include "Game_Defines.h"
#include "Actor.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CTerrainBufferCom;
NS_END

NS_BEGIN(Game)

class CTerrain final : public CActor
{
public:
	typedef struct tagIslandDesc : public ACTOR_DESC
	{
		
	}ISLAND_DESC;
protected:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

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
	CTextureCom* m_pTextureCom[static_cast<uint32_t>(TEXTURE_TYPE::END)] = {nullptr};
	CTerrainBufferCom* m_pTerrainBufferCom = { nullptr };

public:
	static CTerrain* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END