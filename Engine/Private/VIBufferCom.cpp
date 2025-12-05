#include "VIBufferCom.h"
#include "ParticleBufferCom.h"

CVIBufferCom::CVIBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CVIBufferCom::CVIBufferCom(const CVIBufferCom& rhs)
	:CComponent(rhs),
	m_VBuffer(rhs.m_VBuffer), m_IBuffer(rhs.m_IBuffer),

	m_pVertexPositions	(rhs.m_pVertexPositions),
	m_iNumVertexBuffers	(rhs.m_iNumVertexBuffers),
	m_iNumVertices		(rhs.m_iNumVertices), 
	m_iVertexStride		(rhs.m_iVertexStride),
	m_iNumIndices		(rhs.m_iNumIndices), 
	m_iIndexStride		(rhs.m_iIndexStride),
	m_ePrimitiveType	(rhs.m_ePrimitiveType)
{
	Safe_AddRef(m_VBuffer);
	Safe_AddRef(m_IBuffer);
}

HRESULT CVIBufferCom::Initialize_Origin()
{
	return S_OK;
}

HRESULT CVIBufferCom::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CVIBufferCom::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CVIBufferCom::Update(_float fDT)
{
	return 0;
}

void CVIBufferCom::Update_Late(_float fDT)
{
}

HRESULT CVIBufferCom::Bind_Resource()
{
	ID3D11Buffer* pVBuffers[] = {
		m_VBuffer
	};

	uint32_t		iStrides[] = {
		m_iVertexStride
	};

	uint32_t iOffsets[] = {
		0
	};

	m_pContext->IASetVertexBuffers(
		0, 
		m_iNumVertexBuffers, 
		pVBuffers,
		iStrides, 
		iOffsets);

	m_pContext->IASetIndexBuffer(
		m_IBuffer,
		m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 
		0);

	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);

	return S_OK;
}

HRESULT CVIBufferCom::Render()
{
	return S_OK;
}

HRESULT CVIBufferCom::Begin_Draw()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);
	return S_OK;	
}



void CVIBufferCom::Free()
{
	Safe_Release(m_VBuffer);
	Safe_Release(m_IBuffer);

	if(!m_isClone)
		Safe_Delete_Array(m_pVertexPositions);

	CComponent::Free();
}
