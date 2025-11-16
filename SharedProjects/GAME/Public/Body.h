#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CModel;
class CSEModel;
class CShaderCom;
NS_END

NS_BEGIN(Game)

class CBody final : public CPartObject
{
public:
	typedef struct tagBodyDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState = { nullptr };
	}BODY_DESC;

private:
	CBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody(const CBody& rhs); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~CBody() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	const XMFLOAT4X4* Get_SocketMatrix(const _char* pBoneName);

private:
	CModel* m_pModel = { nullptr };
	CSEModel* m_pSeModel = { nullptr };
	CShaderCom* m_pShaderCom = { nullptr };

private:
	const uint32_t* m_pParentState = { nullptr };

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

public:
	static CBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END