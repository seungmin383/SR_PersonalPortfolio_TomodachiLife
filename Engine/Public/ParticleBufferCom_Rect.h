#pragma once
#include"ParticleBufferCom.h"

NS_BEGIN(Engine)

class CParticleBufferCom_Rect final : public CParticleBufferCom
{
private:
	CParticleBufferCom_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CParticleBufferCom_Rect() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	static CParticleBufferCom_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END