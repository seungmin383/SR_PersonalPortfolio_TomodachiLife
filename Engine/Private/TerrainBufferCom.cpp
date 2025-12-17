#include "TerrainBufferCom.h"

CTerrainBufferCom::CTerrainBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBufferCom(pDevice, pContext)
{
}

CTerrainBufferCom::CTerrainBufferCom(const CTerrainBufferCom& rhs)
	:CVIBufferCom(rhs)
{
}

HRESULT CTerrainBufferCom::Initialize_Origin(const _tchar* HeightMapFilePath)
{

	// 받아온 파일 경로로 파일을 만들어주기.
	HANDLE hFile = CreateFile(
		HeightMapFilePath,
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (0 == hFile)
		return E_FAIL;

	// DWORD -> ulong / uint64_t -> u long long
	DWORD dwByte = {};
	// BMP은 FileHeader
	BITMAPFILEHEADER FileHeader = {};
	// InfoHeader
	BITMAPINFOHEADER InfoHeader = {};
	// Pixel로 구성되어 있다.
	uint32_t* Pixels = { nullptr };

	ReadFile(
		hFile,
		&FileHeader,
		sizeof(FileHeader),
		&dwByte,
		nullptr);

	ReadFile(
		hFile,
		&InfoHeader,
		sizeof(InfoHeader),
		&dwByte,
		nullptr);

	uint32_t PixelNumX = InfoHeader.biWidth;
	uint32_t PixelNumZ = InfoHeader.biHeight;

	Pixels = new uint32_t[PixelNumX * PixelNumZ];

	ReadFile(
		hFile,
		Pixels,
		sizeof(uint32_t) * PixelNumX * PixelNumZ,
		&dwByte,
		nullptr);

	CloseHandle(hFile);

	m_iNumVertexBuffers = 1;

	m_iNumVertices = PixelNumX * PixelNumZ;							//정점 개수
	m_iVertexStride = sizeof(VTX_TEX_NORMAL);							//정점 하나의 크기

	m_iNumIndices = ((PixelNumX - 1) * (PixelNumZ - 1) * 3) * 2;	//인덱스 개수
	m_iIndexStride = 4;												//인덱스 하나의 크기

	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pVertexPositions = new _float3[m_iNumVertices];

	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	//Vertex Buffer //====================================================================
	D3D11_BUFFER_DESC VBuffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	VBuffer_Data.Usage = D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	VBuffer_Data.ByteWidth = m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	VBuffer_Data.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	VBuffer_Data.CPUAccessFlags = 0;									//CPU가 접근할 수 있는 권한
	VBuffer_Data.MiscFlags = 0;									//보통 0
	VBuffer_Data.StructureByteStride = m_iVertexStride;						//

	VTX_TEX_NORMAL* pVertices = new VTX_TEX_NORMAL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_TEX_NORMAL) * m_iNumVertices);

	for (uint64_t i = 0; i < PixelNumZ; i++)
	{
		for (uint64_t j = 0; j < PixelNumX; j++)
		{
			uint64_t Index = PixelNumX * i + j;

			m_pVertexPositions[Index] = 
			pVertices[Index].vPos = 
				_float3(static_cast<_float>(j), (Pixels[Index] & 0x000000ff) / 10.f, static_cast<_float>(i));
			pVertices[Index].vTexCoord	= _float2(j / (PixelNumX - 1.f), i / (PixelNumZ - 1.f));

			pVertices[Index].vNormal	= _float3(0.f, 0.f, 0.f);
		}
	}

	//Index Buffer //====================================================================
	D3D11_BUFFER_DESC IBuffer_Data = {};

	IBuffer_Data.Usage = D3D11_USAGE_DEFAULT;
	IBuffer_Data.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBuffer_Data.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBuffer_Data.CPUAccessFlags = 0;
	IBuffer_Data.MiscFlags = 0;
	IBuffer_Data.StructureByteStride = m_iIndexStride;

	uint32_t* pIndices = new uint32_t[m_iNumIndices];

	uint32_t IndicesNum = {};

	for (uint32_t i = 0; i < PixelNumZ - 1; i++)
	{
		for (uint32_t j = 0; j < PixelNumX - 1; j++)
		{
			uint32_t Index = i * PixelNumX + j;

			uint32_t Indices[4] = {
				Index + PixelNumX,
				Index + PixelNumX + 1,
				Index + 1,
				Index
			};

			XMVECTOR vSrc = {};
			XMVECTOR vDst = {};
			XMVECTOR vNormal = {};

			pIndices[IndicesNum++] = Indices[0];
			pIndices[IndicesNum++] = Indices[1];
			pIndices[IndicesNum++] = Indices[2];

			vSrc = XMLoadFloat3(&pVertices[Indices[1]].vPos)
				- XMLoadFloat3(&pVertices[Indices[0]].vPos);

			vDst = XMLoadFloat3(&pVertices[Indices[2]].vPos)
				- XMLoadFloat3(&pVertices[Indices[1]].vPos);

			vNormal = XMVector3Normalize(XMVector3Cross(vSrc, vDst));

			XMStoreFloat3(&pVertices[Indices[0]].vNormal,
				XMLoadFloat3(&pVertices[Indices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[Indices[1]].vNormal,
				XMLoadFloat3(&pVertices[Indices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[Indices[2]].vNormal,
				XMLoadFloat3(&pVertices[Indices[2]].vNormal) + vNormal);

			pIndices[IndicesNum++] = Indices[0];
			pIndices[IndicesNum++] = Indices[2];
			pIndices[IndicesNum++] = Indices[3];

			vSrc = XMLoadFloat3(&pVertices[Indices[2]].vPos)
				- XMLoadFloat3(&pVertices[Indices[0]].vPos);

			vDst = XMLoadFloat3(&pVertices[Indices[3]].vPos)
				- XMLoadFloat3(&pVertices[Indices[2]].vPos);

			vNormal = XMVector3Normalize(XMVector3Cross(vSrc, vDst));

			XMStoreFloat3(&pVertices[Indices[0]].vNormal,
				XMLoadFloat3(&pVertices[Indices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[Indices[3]].vNormal,
				XMLoadFloat3(&pVertices[Indices[3]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[Indices[2]].vNormal,
				XMLoadFloat3(&pVertices[Indices[2]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	D3D11_SUBRESOURCE_DATA VBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	VBuffer_Init_Data.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(
		&VBuffer_Data,
		&VBuffer_Init_Data,
		&m_VBuffer)))
	{
		MSG_BOX("ErrCode : Vertex Buffer Create Failed");
		return E_FAIL;
	}

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

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(Pixels);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CTerrainBufferCom::Initialize_Clone(void* pArg)
{
	return S_OK;
}


CTerrainBufferCom* CTerrainBufferCom::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext,
	const _tchar* HeightMapFilePath)
{
	CTerrainBufferCom* pInstance = new CTerrainBufferCom(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin(HeightMapFilePath)))
	{
		MSG_BOX("ErrCode : RectBufferCom Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}



CObject* CTerrainBufferCom::Clone(void* pArg)
{
	CTerrainBufferCom* pInstance = new CTerrainBufferCom(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : RectBufferCom Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTerrainBufferCom::Free()
{
	CVIBufferCom::Free();
}
