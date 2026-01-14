#pragma once
#include "Game_Defines.h"
#include "Actor.h"

 //#define LOADING_MODEL 0

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CModel;
class CSEModel;
class CAnimationPlayer;

NS_END

NS_BEGIN(Game)
class CSEMii;

class CRoom final : public CActor
{
public:
	typedef struct tagRoomDesc : public ACTOR_DESC
	{

	}ROOM_DESC;

protected:
	CRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoom(const CRoom& rhs);
	virtual ~CRoom() = default;

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

public:
	void Set_Owner(CSEMii* pMii) { m_pOwner = pMii; }
	CSEMii* Get_Owner() { return m_pOwner; }

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CSEModel* m_pSeModel = { nullptr };

	CSEMii* m_pOwner = { nullptr };

public:
	static CRoom* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END