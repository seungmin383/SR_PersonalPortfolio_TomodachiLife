#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CSEModel;
class CShaderCom;
NS_END

NS_BEGIN(Game)

class CMiiHair final : public CPartObject
{
public:
	typedef struct tagMiiHair : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState;
		const XMFLOAT4X4* pSocketMatrix;

	}MIIHAIR_DESC;

private:
	CMiiHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiiHair(const CMiiHair& rhs);
	virtual ~CMiiHair() = default;

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
	CSEModel* m_pSeModel[10] = {nullptr};
	CShaderCom* m_pShader = { nullptr };

private:
	const uint32_t* m_pParentState = { nullptr };
	const XMFLOAT4X4* m_pSocketMatrix = { nullptr };
	XMFLOAT4 m_vHairColor = {};

	uint32_t m_iCurModelIndex = {};

public:
	static CMiiHair* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END