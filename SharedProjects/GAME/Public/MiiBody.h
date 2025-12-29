#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CSEModel;
class CShaderCom;

class CAnimationPlayer;
NS_END

NS_BEGIN(Game)

class CMiiBody final : public CPartObject
{
public:
	typedef struct tagMiiBodyDesc : public CPartObject::PARTOBJECT_DESC
	{
		const int32_t* pParnetState;
		const int32_t* pParentClothState;

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

public:
	const XMFLOAT4X4* Get_SocketMatrix(const _char* pBoneName);

private:
	CSEModel* m_pSeModel = { nullptr };
	CShaderCom* m_pShader = { nullptr };
	CAnimationPlayer* m_pAnimationPlayer = { nullptr };

private:
	const int32_t* m_pParentState = { nullptr };
	const int32_t* m_pParentClothState = { nullptr };

public:
	static CMiiBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END