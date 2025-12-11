#pragma once
#include"ParticleBufferCom.h"

NS_BEGIN(Engine)

class ENGINE_DLL CParticleBufferCom_Point final : public CParticleBufferCom
{
public:
	typedef struct tagParticleBufferPointDesc : public CParticleBufferCom::PARTICLE_DESC
	{

	}PARTICLE_POINT_DESC;

private:
	CParticleBufferCom_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticleBufferCom_Point(const CParticleBufferCom_Point& rhs);
	virtual ~CParticleBufferCom_Point() = default;

public:
	virtual HRESULT Initialize_Origin(const PARTICLE_DESC* pParticleDesc);
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual HRESULT Bind_Resource()override;
	virtual HRESULT Begin_Draw()override;

public:
	static CParticleBufferCom_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC* pParticleDesc);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END