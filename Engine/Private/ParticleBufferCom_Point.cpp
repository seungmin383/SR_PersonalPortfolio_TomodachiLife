#include "ParticleBufferCom_Point.h"
#include "GameInstance.h"

CParticleBufferCom_Point::CParticleBufferCom_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticleBufferCom(pDevice, pContext)
{
}

CParticleBufferCom_Point::CParticleBufferCom_Point(const CParticleBufferCom_Point& rhs)
	:CParticleBufferCom(rhs)
{
}

HRESULT CParticleBufferCom_Point::Initialize_Origin(const PARTICLE_DESC* pParticleDesc)
{
	/* 버텍스 버퍼 갯수 */
	m_iNumVertexBuffers = 2;

	/* 정점 갯수 */
	m_iNumVertices = 1;

	/* 정점 하나의 크기 */
	m_iVertexStride = sizeof(VTX_POS);

	/* 인덱스 갯수 */
	m_iNumIndices = 0;

	/* 인덱스 크기 */
	m_iIndexStride = 0;

	/* 어떻게 그릴지 */
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	const PARTICLE_POINT_DESC* pDesc = static_cast<const PARTICLE_POINT_DESC*>(pParticleDesc);
	m_iInstVertexStride = sizeof(VTX_PARTICLE_INST);
	m_iNumInst = pDesc->iNumInst;
	m_iIndexCountPerInst = m_iNumIndices;

	/* 버텍스 버퍼 */
	{
		m_pVertexPositions = new _float3[m_iNumVertices];
		ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

		VTX_POS* pVertices = new VTX_POS[m_iNumVertices];
		ZeroMemory(pVertices, sizeof(VTX_POS) * m_iNumVertices);

		D3D11_BUFFER_DESC VBDesc = {};

		VBDesc.Usage = D3D11_USAGE_DEFAULT;
		VBDesc.MiscFlags = 0;
		VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
		VBDesc.CPUAccessFlags = 0;
		VBDesc.StructureByteStride = m_iVertexStride;

		m_pVertexPositions[0] = pVertices[0].vPos = _float3(0.f, 0.f, 0.f);

		D3D11_SUBRESOURCE_DATA VB_InitData = {};
		VB_InitData.pSysMem = pVertices;

		if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &VB_InitData, &m_VBuffer)))
		{
			MSG_BOX("ErrCode : Vertex Buffer Create Failed");
			Safe_Delete_Array(pVertices);
			return E_FAIL;
		}
		Safe_Delete_Array(pVertices);
	}

	/* 인덱스 버퍼 */
	

	/* 인스턴싱 버퍼 */
	{
		m_InstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_InstBufferDesc.MiscFlags = 0;
		m_InstBufferDesc.ByteWidth = m_iInstVertexStride * m_iNumInst;
		m_InstBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_InstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_InstBufferDesc.StructureByteStride = m_iInstVertexStride;

		m_pInstVertices = new VTX_PARTICLE_INST[m_iNumInst];
		ZeroMemory(m_pInstVertices, sizeof(VTX_PARTICLE_INST) * m_iNumInst);

		m_pSpeed = new _float[m_iNumInst];
		ZeroMemory(m_pSpeed, sizeof(_float) * m_iNumInst);

		m_isLoop = pDesc->isLoop;
		m_vPivot = pDesc->vPivot;

		for (uint32_t i = 0; i < m_iNumInst; ++i)
		{
			_float  fScale = m_pGameInstance->Random(pParticleDesc->vSize.x, pParticleDesc->vSize.y);

			m_pSpeed[i] = m_pGameInstance->Random(pDesc->vSpeed.x, pDesc->vSpeed.y);

			m_pInstVertices[i].vRight = XMFLOAT4(fScale, 0.f, 0.f, 0.f);
			m_pInstVertices[i].vUp = XMFLOAT4(0.f, fScale, 0.f, 0.f);
			m_pInstVertices[i].vLook = XMFLOAT4(0.f, 0.f, fScale, 0.f);
			m_pInstVertices[i].vTranslation = XMFLOAT4(
				m_pGameInstance->Random(pParticleDesc->vCenter.x - pParticleDesc->vRange.x * 0.5f, pParticleDesc->vCenter.x + pParticleDesc->vRange.x * 0.5f),
				m_pGameInstance->Random(pParticleDesc->vCenter.y - pParticleDesc->vRange.y * 0.5f, pParticleDesc->vCenter.y + pParticleDesc->vRange.y * 0.5f),
				m_pGameInstance->Random(pParticleDesc->vCenter.z - pParticleDesc->vRange.z * 0.5f, pParticleDesc->vCenter.z + pParticleDesc->vRange.z * 0.5f),
				1.f);

			m_pInstVertices[i].vLifeTime = XMFLOAT2(0.f, m_pGameInstance->Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y));
		}
	}
	return S_OK;
}

HRESULT CParticleBufferCom_Point::Initialize_Clone(void* pArg)
{
	D3D11_SUBRESOURCE_DATA      InstB_InitData{};
	InstB_InitData.pSysMem = m_pInstVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_InstBufferDesc, &InstB_InitData, &m_pVBInst)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleBufferCom_Point::Bind_Resource()
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
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);

	return S_OK;
}

HRESULT CParticleBufferCom_Point::Begin_Draw()
{
	m_pContext->DrawInstanced(1,m_iNumInst, 0, 0);
	return S_OK;
}

CParticleBufferCom_Point* CParticleBufferCom_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC* pParticleDesc)
{
	CParticleBufferCom_Point* pInstance = new CParticleBufferCom_Point(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin(pParticleDesc)))
	{
		MSG_BOX("ErrCode :CParticleBufferCom_Point Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CParticleBufferCom_Point::Clone(void* pArg)
{
	CParticleBufferCom_Point* pInstance = new CParticleBufferCom_Point(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode :CParticleBufferCom_Point Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CParticleBufferCom_Point::Free()
{
	CParticleBufferCom::Free();
}
