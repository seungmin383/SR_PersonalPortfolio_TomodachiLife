#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)

class CSEMiiMouth final : public CPartObject
{
public:
	typedef struct tagSEMiiMouthDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState;
		const XMFLOAT4X4* pSocketMatrix;

		MIIMOUTHINFO_DESC* pMouthInfo = { nullptr };

	}SEMIIMOUTH_DESC;

private:
	CSEMiiMouth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEMiiMouth(const CSEMiiMouth& rhs);
	virtual ~CSEMiiMouth() = default;

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
	CShaderCom* m_pShader = { nullptr };
	CTextureCom* m_pTexture = { nullptr };
	CRectBufferCom* m_pRectBuffer = { nullptr };

private:
	const uint32_t* m_pParentState = { nullptr };
	const XMFLOAT4X4* m_pSocketMatrix = { nullptr };

	uint32_t m_iTextureIndex = {};

public:
	static CSEMiiMouth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END