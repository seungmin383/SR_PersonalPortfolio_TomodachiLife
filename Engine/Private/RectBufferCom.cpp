#include "RectBufferCom.h"

CRectBufferCom::CRectBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CRectBufferCom::CRectBufferCom(const CRectBufferCom& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CRectBufferCom::Initialize_Origin()
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices	= 4;				//정점 개수
	m_iVertexStride = sizeof(VTX_TEX);	//정점 하나의 크기

	m_iNumIndices	= 6;				//인덱스 개수
	m_iIndexStride	= 2;				//인덱스 하나의 크기

	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//Vertex Buffer //====================================================================
	D3D11_BUFFER_DESC Vtx_Buffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	Vtx_Buffer_Data.Usage				= D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	Vtx_Buffer_Data.ByteWidth			= m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	Vtx_Buffer_Data.BindFlags			= D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	Vtx_Buffer_Data.CPUAccessFlags		= 0;									//CPU가 접근할 수 있는 권한
	Vtx_Buffer_Data.MiscFlags			= 0;									//보통 0
	Vtx_Buffer_Data.StructureByteStride = m_iVertexStride;						//

	VTX_TEX* pVertices = new VTX_TEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_TEX) * m_iNumVertices);


	pVertices[0].vPos		= _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexCoord	= _float2(0.f, 0.f);

	pVertices[1].vPos		= _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexCoord	= _float2(1.f, 0.f);

	pVertices[2].vPos		= _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexCoord	= _float2(1.f, 1.f);

	pVertices[3].vPos		= _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexCoord	= _float2(0.f, 1.f);

	D3D11_SUBRESOURCE_DATA Vtx_Buffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	Vtx_Buffer_Init_Data.pSysMem = pVertices;
	Safe_Delete_Array(pVertices);

	if(FAILED(m_pDevice->CreateBuffer(&Vtx_Buffer_Data, &Vtx_Buffer_Init_Data, &m_VB)))
	{
		MSG_BOX("ErrCode : Vertex Buffer Create Failed");
		return E_FAIL;
	}

	//Index Buffer //====================================================================
	D3D11_BUFFER_DESC Idx_Buffer_Data = {};

	Idx_Buffer_Data.Usage				= D3D11_USAGE_DEFAULT;					
	Idx_Buffer_Data.ByteWidth			= m_iIndexStride * m_iNumIndices;
	Idx_Buffer_Data.BindFlags			= D3D11_BIND_INDEX_BUFFER;			
	Idx_Buffer_Data.CPUAccessFlags		= 0;								
	Idx_Buffer_Data.MiscFlags			= 0;									
	Idx_Buffer_Data.StructureByteStride = m_iIndexStride;			

	uint16_t* pIndices = new uint16_t[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(uint16_t) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	
	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	D3D11_SUBRESOURCE_DATA Ind_Buffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	Ind_Buffer_Init_Data.pSysMem = pIndices;
	Safe_Delete_Array(pIndices);

	if (FAILED(m_pDevice->CreateBuffer(&Idx_Buffer_Data, &Ind_Buffer_Init_Data, &m_IB)))
	{
		MSG_BOX("ErrCode : Index Buffer Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRectBufferCom::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CRectBufferCom::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CRectBufferCom::Update(const _float _fDT)
{
	return 0;
}

void CRectBufferCom::Update_Late(const _float _fDT)
{
}

HRESULT CRectBufferCom::Render()
{
	return S_OK;
}

CRectBufferCom* CRectBufferCom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRectBufferCom* pInstance = new CRectBufferCom(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : RectBufferCom Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CObject* CRectBufferCom::Clone(void* pArg)
{
	CRectBufferCom* pInstance = new CRectBufferCom(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : RectBufferCom Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRectBufferCom::Free()
{
	CVIBuffer::Free();
}
