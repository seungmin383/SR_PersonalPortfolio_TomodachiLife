#pragma once
#include"ParticleBufferCom.h"

NS_BEGIN(Engine)

class CParticleBufferCom_Rect final : public CParticleBufferCom
{
public:
	typedef struct tagParticleBufferRectDesc : public CParticleBufferCom::PARTICLE_DESC
	{

	}PARTICLE_RECT_DESC;

private:
	CParticleBufferCom_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CParticleBufferCom_Rect() = default;

public:
	virtual HRESULT Initialize_Origin(const PARTICLE_DESC* pParticleDesc);
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	static CParticleBufferCom_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC* pParticleDesc);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END