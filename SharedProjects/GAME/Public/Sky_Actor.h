#pragma once
#include "Game_Defines.h"
#include "Actor.h"


NS_BEGIN(Engine)

class CShaderCom;
class CSEModel;
class CAnimationPlayer;

NS_END

NS_BEGIN(Game)

class CSky_Actor final : public CActor
{
public:
	typedef struct tagSkyDesc : public ACTOR_DESC
	{
	}SKY_DESC;

protected:
	CSky_Actor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSky_Actor(const CSky_Actor& rhs);
	virtual ~CSky_Actor() = default;

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
	CSEModel* m_pSeModel = { nullptr };

	CAnimationPlayer* m_pAnimationPlayer = { nullptr };

public:
	static CSky_Actor* Create(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END