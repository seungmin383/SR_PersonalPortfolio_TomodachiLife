#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)

class CSEMiiEyebrow final : public CPartObject
{
public:
	typedef struct tagSEMiiEyebrowDesc : public CPartObject::PARTOBJECT_DESC
	{
		const int32_t* pParentState;
		vector<const XMFLOAT4X4* >* pSocketMatrix;

		_bool isRight;

		MIIEYEBROWINFO_DESC* pEyebrowInfo = { nullptr };

	}SEMIIEYEBROW_DESC;

private:
	CSEMiiEyebrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEMiiEyebrow(const CSEMiiEyebrow& rhs);
	virtual ~CSEMiiEyebrow() = default;

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
	const int32_t* m_pParentState = { nullptr };
	vector<const XMFLOAT4X4*>* m_pSocketMatrix = { nullptr };

	XMFLOAT4 m_vEyebrowColor = {};

	uint32_t m_iTextureIndex = {};

	/* 오른쪽 눈 인지 */
	_bool m_isRight = { FALSE };
public:
	static CSEMiiEyebrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END