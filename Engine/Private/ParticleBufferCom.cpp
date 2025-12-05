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
	ID3D11Buffer* pVertexBuffers[] = {
		m_VBuffer,
		m_pVBInst
	};

	_uint		 iVertexStrides[] = {
		m_iVertexStride,
		m_iInstVertexStride,
	};

	_uint		 iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_IBuffer, m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);
	
	return S_OK;
}

HRESULT CParticleBufferCom::Render()
{
	return S_OK;
}

HRESULT CParticleBufferCom::Begin_Draw()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInst, m_iNumInst, 0, 0, 0);
	return S_OK;
}

void CParticleBufferCom::Free()
{
	CVIBufferCom::Free();
}