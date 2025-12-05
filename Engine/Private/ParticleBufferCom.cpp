#include "ParticleBufferCom.h"


CParticleBufferCom::CParticleBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBufferCom(pDevice, pContext)
{
}

HRESULT CParticleBufferCom::Initialize_Origin()
{
	return S_OK;
}

HRESULT CParticleBufferCom::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CParticleBufferCom::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CParticleBufferCom::Update(_float fDT)
{
	return 0;
}

void CParticleBufferCom::Update_Late(_float fDT)
{
}

HRESULT CParticleBufferCom::Bind_Resource()
{
	return S_OK;
}

HRESULT CParticleBufferCom::Render()
{
	return S_OK;
}

HRESULT CParticleBufferCom::Begin_Draw()
{
	return S_OK;
}

void CParticleBufferCom::Free()
{
	CVIBufferCom::Free();
}