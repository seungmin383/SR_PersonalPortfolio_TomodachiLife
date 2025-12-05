#pragma once
#include "VIBufferCom.h"

NS_BEGIN(Engine)

class CParticleBufferCom abstract : public CVIBufferCom
{
protected:
	CParticleBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
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
	virtual void Free()override;
};

NS_END