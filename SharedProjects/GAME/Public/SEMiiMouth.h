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
		const int32_t* pParentState;
		vector<const XMFLOAT4X4* >* pSocketMatrix;

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
	const int32_t* m_pParentState = { nullptr };
	vector<const XMFLOAT4X4*>* m_pSocketMatrix = { nullptr };

	uint32_t m_iTextureIndex = {};

	/* 열 타이밍 */
	_float m_fOpenTimeAcc = {};
	_float m_fOpenMouthTimer = { 0.5f };

	/* 열고 있을 시간 */
	_float m_fOpeningTimeAcc = {};
	_float m_fOpeningMouthTimer = { 0.3f };

	_bool m_isOpeing = { FALSE };

	uint32_t m_iOpenMouthTextureIndex = { 108 };
	uint32_t m_iCloseMouthTextureIndex = { 101 };
	uint32_t m_iHappyMouthTextureIndex = { 102 };


public:
	static CSEMiiMouth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END