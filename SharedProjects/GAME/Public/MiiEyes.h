#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)

class CMiiEyes final : public CPartObject
{
public:
	typedef struct tagMiiEyesDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState;
		const XMFLOAT4X4* pSocketMatrix;

		_float fBlinkTime = {};
		_float fBlinkDuration = { 0.3f };

		_bool isRight;

	}MIIEYES_DESC;

private:
	CMiiEyes(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiiEyes(const CMiiEyes& rhs);
	virtual ~CMiiEyes() = default;

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
	void Ready_Timer();

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
	_float m_fBlinkTime = {};
	_float m_fBlinkTimer = {};
	_float m_fBlinkDuration = {};
	_float m_fBlinkDurationTimer = {};
	
	/* 오른쪽 눈 인지 */
	_bool m_isRight = { FALSE };
	_float m_fAngelX = { 0.f };
	_float m_fAngelY = { 0.f };
	_float m_fAngelZ = { 0.f };
public:
	static CMiiEyes* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END