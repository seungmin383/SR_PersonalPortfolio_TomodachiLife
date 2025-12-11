#pragma once
#include "VIBufferCom.h"

NS_BEGIN(Engine)

class ENGINE_DLL CParticleBufferCom abstract : public CVIBufferCom
{
public:
	typedef struct tagParticleBufferDesc
	{
		uint32_t iNumInst = {};
		XMFLOAT2 vSize = {};
		XMFLOAT3 vCenter = {};
		XMFLOAT3 vRange = {};

		XMFLOAT3 vPivot = {};
		XMFLOAT2 vSpeed = {};
		XMFLOAT2 vLifeTime = {};
		
		_bool isLoop = {};

	}PARTICLE_DESC;

protected:
	CParticleBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticleBufferCom(const CParticleBufferCom& rhs);
	virtual ~CParticleBufferCom() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Bind_Resource();
	virtual HRESULT Render()override;

	virtual HRESULT Begin_Draw()override;

public:
	void Drop(_float fDT);
	void Spread(_float fDT);

protected:
	ID3D11Buffer* m_pVBInst = { nullptr };
	D3D11_BUFFER_DESC m_InstBufferDesc = {};

protected:
	VTX_PARTICLE_INST* m_pInstVertices = { nullptr };
	uint32_t m_iIndexCountPerInst = {};
	uint32_t m_iNumInst = {};
	uint32_t m_iInstVertexStride = {};

	_float* m_pSpeed = { nullptr };
	_bool m_isLoop = { FALSE };
	XMFLOAT3 m_vPivot = {};

public:
	virtual void Free()override;
};

NS_END