#include "ParticleBufferCom_Rect.h"

CParticleBufferCom_Rect::CParticleBufferCom_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticleBufferCom(pDevice, pContext)
{
}

HRESULT CParticleBufferCom_Rect::Initialize_Origin()
{
	return S_OK;
}

HRESULT CParticleBufferCom_Rect::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CParticleBufferCom_Rect* CParticleBufferCom_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticleBufferCom_Rect* pInstance = new CParticleBufferCom_Rect(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode :CParticleBufferCom_Rect Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CParticleBufferCom_Rect::Clone(void* pArg)
{
	CParticleBufferCom_Rect* pInstance = new CParticleBufferCom_Rect(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode :CParticleBufferCom_Rect Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CParticleBufferCom_Rect::Free()
{
}
