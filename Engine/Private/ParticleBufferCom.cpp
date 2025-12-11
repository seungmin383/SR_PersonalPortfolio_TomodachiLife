#include "ParticleBufferCom.h"


CParticleBufferCom::CParticleBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBufferCom(pDevice, pContext)
{
}

CParticleBufferCom::CParticleBufferCom(const CParticleBufferCom& rhs)
	:CVIBufferCom(rhs),
	m_pVBInst(rhs.m_pVBInst),
	m_InstBufferDesc(rhs.m_InstBufferDesc),
	m_pInstVertices(rhs.m_pInstVertices),
	m_iIndexCountPerInst(rhs.m_iIndexCountPerInst),
	m_iNumInst(rhs.m_iNumInst),
	m_iInstVertexStride(rhs.m_iInstVertexStride),
	m_pSpeed(rhs.m_pSpeed),
	m_isLoop(rhs.m_isLoop),
	m_vPivot(rhs.m_vPivot)
{
	Safe_AddRef(m_pVBInst);
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

void CParticleBufferCom::Drop(_float fDT)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	/* 내가 가지고 있는 인스턴싱 데이터를 열어서 */
	m_pContext->Map(m_pVBInst, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	/* 지역변수에 주소를 저장 -> 값 변경용 */
	VTX_PARTICLE_INST* pVertices = static_cast<VTX_PARTICLE_INST*>(SubResource.pData);

	for (uint32_t i = 0; i < m_iNumInst; ++i)
	{
		pVertices[i].vTranslation.y -= m_pSpeed[i] * fDT;
		pVertices[i].vLifeTime.x += fDT;

		if (m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vTranslation = m_pInstVertices[i].vTranslation;
			pVertices[i].vLifeTime.x = 0.f;
		}
	}

	m_pContext->Unmap(m_pVBInst, 0);
}

void CParticleBufferCom::Spread(_float fDT)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	/* 내가 가지고 있는 인스턴싱 데이터를 열어서 */
	m_pContext->Map(m_pVBInst, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	/* 지역변수에 주소를 저장 -> 값 변경용 */
	VTX_PARTICLE_INST* pVertices = static_cast<VTX_PARTICLE_INST*>(SubResource.pData);

	for (uint32_t i = 0; i < m_iNumInst; ++i)
	{
		//XMVECTOR vLook = XMVector4Normalize(XMVectorSetW(XMLoadFloat3(&m_vPivot) - XMLoadFloat4(&pVertices[i].vTranslation), 0.f)) * m_pSpeed[i];
		//XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + vLook * fDT);
		
		XMVECTOR		vLook = XMVector3Normalize(XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f)) * m_pSpeed[i];

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + vLook * fDT);
		pVertices[i].vLifeTime.x += fDT;

		if (m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vTranslation = m_pInstVertices[i].vTranslation;
			pVertices[i].vLifeTime.x = 0.f;
		}
	}

	m_pContext->Unmap(m_pVBInst, 0);
}

void CParticleBufferCom::Free()
{
	if (!m_isClone)
	{
		Safe_Delete_Array(m_pSpeed);
		Safe_Delete_Array(m_pInstVertices);
	}
	Safe_Release(m_pVBInst);

	CVIBufferCom::Free();
}