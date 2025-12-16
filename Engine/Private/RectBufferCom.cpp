#include "RectBufferCom.h"

CRectBufferCom::CRectBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBufferCom(pDevice, pContext)
{
}

CRectBufferCom::CRectBufferCom(const CRectBufferCom& rhs)
	:CVIBufferCom(rhs)
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
	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

//Vertex Buffer //====================================================================
	D3D11_BUFFER_DESC VBuffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	VBuffer_Data.Usage				= D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	VBuffer_Data.ByteWidth			= m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	VBuffer_Data.BindFlags			= D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	VBuffer_Data.CPUAccessFlags		= 0;									//CPU가 접근할 수 있는 권한
	VBuffer_Data.MiscFlags			= 0;									//보통 0
	VBuffer_Data.StructureByteStride = m_iVertexStride;						//

	VTX_TEX* pVertices = new VTX_TEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_TEX) * m_iNumVertices);

	m_pVertexPositions[0] =
	pVertices[0].vPos		= _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexCoord	= _float2(0.f, 0.f);

	m_pVertexPositions[1] =
	pVertices[1].vPos		= _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexCoord	= _float2(1.f, 0.f);

	m_pVertexPositions[2] =
	pVertices[2].vPos		= _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexCoord	= _float2(1.f, 1.f);

	m_pVertexPositions[3] =
	pVertices[3].vPos		= _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexCoord	= _float2(0.f, 1.f);

	D3D11_SUBRESOURCE_DATA VBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	VBuffer_Init_Data.pSysMem = pVertices;

	if(FAILED(m_pDevice->CreateBuffer(
		&VBuffer_Data, 
		&VBuffer_Init_Data, 
		&m_VBuffer)))
	{
		MSG_BOX("ErrCode : Vertex Buffer Create Failed");
		return E_FAIL;
	}

	Safe_Delete_Array(pVertices);

//Index Buffer //====================================================================
	D3D11_BUFFER_DESC IBuffer_Data = {};

	IBuffer_Data.Usage				= D3D11_USAGE_DEFAULT;					
	IBuffer_Data.ByteWidth			= m_iIndexStride * m_iNumIndices;
	IBuffer_Data.BindFlags			= D3D11_BIND_INDEX_BUFFER;			
	IBuffer_Data.CPUAccessFlags		= 0;								
	IBuffer_Data.MiscFlags			= 0;									
	IBuffer_Data.StructureByteStride = m_iIndexStride;			

	uint16_t* pIndices = new uint16_t[m_iNumIndices];

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	
	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	D3D11_SUBRESOURCE_DATA IBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	IBuffer_Init_Data.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(
		&IBuffer_Data,
		&IBuffer_Init_Data,
		&m_IBuffer)))
	{
		MSG_BOX("ErrCode : Index Buffer Create Failed");
		return E_FAIL;
	}

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CRectBufferCom::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CRectBufferCom* CRectBufferCom::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext)
{
	CRectBufferCom* pInstance = new CRectBufferCom(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : RectBufferCom Create Failed");
		Safe_Release(pInstance);
		return nullptr;
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
		return nullptr;
	}

	return pInstance;
}

void CRectBufferCom::Free()
{
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	CVIBufferCom::Free();
}
