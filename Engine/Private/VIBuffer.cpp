#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	:CComponent(rhs),
	m_VB(rhs.m_VB), m_IB(rhs.m_IB),

	m_pVertexPositions	(rhs.m_pVertexPositions),
	m_iNumVertexBuffers	(rhs.m_iNumVertexBuffers),
	m_iNumVertices		(rhs.m_iNumVertices), 
	m_iVertexStride		(rhs.m_iVertexStride),
	m_iNumIndices		(rhs.m_iNumIndices), 
	m_iIndexStride		(rhs.m_iIndexStride)
{
	Safe_AddRef(m_VB);
	Safe_AddRef(m_IB);
}

HRESULT CVIBuffer::Initialize_Origin()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CVIBuffer::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CVIBuffer::Update(const _float _fDT)
{
	return 0;
}

void CVIBuffer::Update_Late(const _float _fDT)
{
}

HRESULT CVIBuffer::Bind_Resource()
{
	ID3D11Buffer* pVBs[] = {
		m_VB 
	};

	uint32_t		iStrides[] = {
		m_iVertexStride
	};

	uint32_t iOffsets[] = {
		0
	};

	// void ID3D11DeviceContext::IASetVertexBuffers(
	// 	UINT StartSlot,							//정점 버퍼들을 붙이기 시작할 입력 슬롯의 인덱스 번호 / 총 32개가 있다.
	// 	UINT NumBuffers,						//입력 슬롯들에 바인딩하고자 하는 버퍼들의 개수
	// 	ID3D11Buffer* const* ppVertexBuffers,	//내가 바인딩할 버퍼들을 담은 배열의 첫 원소를 가리키는 포인터
	// 	const UINT* pStrides,					//stride : 해당 정점 버퍼의 한 원소의 바이트 단위 크기, 이러한 stride들을 담은 배열의 첫 원소를 가리키는 포인터
	// 	const UINT* pOffsets);					//보통 0을 사용한다 / 각 버퍼의 첫 정점이 시작되는 위치를 바이트 단위로 나타낸다. 이러한 offset들을 담은 배열의 첫 원소를 가리키는 포인터

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVBs, iStrides, iOffsets);

	m_pContext->IASetIndexBuffer(m_IB, m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);

	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);
	return S_OK;
}

void CVIBuffer::Free()
{
	Safe_Release(m_VB);
	Safe_Release(m_IB);

	if(!m_isClone && m_pVertexPositions)
		Safe_Delete_Array(m_pVertexPositions);

	CComponent::Free();
}
