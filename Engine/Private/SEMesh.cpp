#include "SEMesh.h"
#include "ShaderCom.h"
#include "SEBone.h"

CSEMesh::CSEMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext )
	:CVIBufferCom(pDevice, pContext)
{
}

HRESULT CSEMesh::Initialize_Mesh(SE_MESH& seMesh, vector<CSEBone*>& pBones)
{
	 m_strName = seMesh.strName;	
	 if (m_strName.find("Alpha") != std::string::npos)
		 m_isAlpha = TRUE;

	 if (m_strName.find("Light") != std::string::npos)
		 m_isLight = TRUE;

	 if (m_strName.find("Shadow") != std::string::npos)
		 m_isShadow = TRUE;



	 m_iMaterialIndex = seMesh.iMaterialIndex;
	 m_BoneIndices = seMesh.BoneIndices;
	 m_Vertices = seMesh.Vertices;
	 m_Indices = seMesh.Indices;


	 m_iNumVertexBuffers = 1;
	 m_iNumVertices = static_cast<uint32_t>(m_Vertices.size());						//정점 개수
	 m_iNumIndices = static_cast<uint32_t>(m_Indices.size());
	 m_iIndexStride = 4;												//인덱스 하나의 크기

	 m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	 m_iVertexStride = sizeof(VTX_TEX_MESH);							//정점 하나의 크기
	 D3D11_BUFFER_DESC VBuffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	 VBuffer_Data.Usage = D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	 VBuffer_Data.ByteWidth = m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	 VBuffer_Data.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	 VBuffer_Data.CPUAccessFlags = 0;									//CPU가 접근할 수 있는 권한
	 VBuffer_Data.MiscFlags = 0;									//보통 0
	 VBuffer_Data.StructureByteStride = m_iVertexStride;						//


	 D3D11_SUBRESOURCE_DATA VBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	 VBuffer_Init_Data.pSysMem = m_Vertices.data();

	 if (FAILED(m_pDevice->CreateBuffer(
		 &VBuffer_Data,
		 &VBuffer_Init_Data,
		 &m_VBuffer)))
	 {
		 MSG_BOX("ErrCode : Vertex Buffer Create Failed");
		 return E_FAIL;
	 }




	 // INDEX BUFFER
	 D3D11_BUFFER_DESC IBuffer_Data = {};

	 IBuffer_Data.Usage = D3D11_USAGE_DEFAULT;
	 IBuffer_Data.ByteWidth = m_iIndexStride * m_iNumIndices;
	 IBuffer_Data.BindFlags = D3D11_BIND_INDEX_BUFFER;
	 IBuffer_Data.CPUAccessFlags = 0;
	 IBuffer_Data.MiscFlags = 0;
	 IBuffer_Data.StructureByteStride = m_iIndexStride;

	 D3D11_SUBRESOURCE_DATA IBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	 IBuffer_Init_Data.pSysMem = m_Indices.data();

	 if (FAILED(m_pDevice->CreateBuffer(
		 &IBuffer_Data,
		 &IBuffer_Init_Data,
		 &m_IBuffer)))
	 {
		 MSG_BOX("ErrCode : Index Buffer Create Failed");
		 return E_FAIL;
	 }

	 return S_OK;
}

HRESULT CSEMesh::Initialize_AnimMesh(SE_ANIM_MESH& seAnimMesh, vector<CSEBone*>& pBones)
{
	m_strName = seAnimMesh.strName;
	if (m_strName.find("Alpha") != std::string::npos)
		m_isAlpha = TRUE;

	if (m_strName.find("Light") != std::string::npos)
		m_isLight = TRUE;

	if (m_strName.find("Shadow") != std::string::npos)
		m_isShadow = TRUE;

	if (m_strName.find("SeaMaskMesh") != std::string::npos)
		m_isSea = TRUE;

	m_iMaterialIndex = seAnimMesh.iMaterialIndex;
	m_BoneIndices = seAnimMesh.BoneIndices;
	m_AnimVertices = seAnimMesh.Vertices;
	m_Indices = seAnimMesh.Indices;

	uint32_t iNumBones = static_cast<uint32_t>(m_BoneIndices.size());
	m_OffsetMatrices.resize(iNumBones);
	for (uint32_t i = 0; i < iNumBones; ++i)
	{
		XMStoreFloat4x4(
			&m_OffsetMatrices[i], pBones[m_BoneIndices[i]]->Get_OffsetMatrix());
	}

	m_iNumVertexBuffers = 1;
	m_iNumVertices = static_cast<uint32_t>(m_AnimVertices.size());						//정점 개수
	m_iNumIndices = static_cast<uint32_t>(m_Indices.size());
	m_iIndexStride = 4;

	m_iVertexStride = sizeof(VTX_TEX_ANIM_MESH);							//정점 하나의 크기
	D3D11_BUFFER_DESC VBuffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	VBuffer_Data.Usage = D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	VBuffer_Data.ByteWidth = m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	VBuffer_Data.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	VBuffer_Data.CPUAccessFlags = 0;									//CPU가 접근할 수 있는 권한
	VBuffer_Data.MiscFlags = 0;									//보통 0
	VBuffer_Data.StructureByteStride = m_iVertexStride;						//

	D3D11_SUBRESOURCE_DATA VBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	VBuffer_Init_Data.pSysMem = m_AnimVertices.data();

	if (FAILED(m_pDevice->CreateBuffer(
		&VBuffer_Data,
		&VBuffer_Init_Data,
		&m_VBuffer)))
	{
		MSG_BOX("ErrCode : Vertex Buffer Create Failed");
		return E_FAIL;
	}


	// INDEX BUFFER
	D3D11_BUFFER_DESC IBuffer_Data = {};

	IBuffer_Data.Usage = D3D11_USAGE_DEFAULT;
	IBuffer_Data.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBuffer_Data.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBuffer_Data.CPUAccessFlags = 0;
	IBuffer_Data.MiscFlags = 0;
	IBuffer_Data.StructureByteStride = m_iIndexStride;

	D3D11_SUBRESOURCE_DATA IBuffer_Init_Data = {}; //초기화 데이터를 설정해주는 것
	IBuffer_Init_Data.pSysMem = m_Indices.data();

	if (FAILED(m_pDevice->CreateBuffer(
		&IBuffer_Data,
		&IBuffer_Init_Data,
		&m_IBuffer)))
	{
		MSG_BOX("ErrCode : Index Buffer Create Failed");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSEMesh::Bind_Bones(
	CShaderCom* pShader, 
	const _char* pConstantName, 
	vector<CSEBone*>& pBones)
{
	uint32_t iNumBone = static_cast<uint32_t>(m_BoneIndices.size());

	for (uint32_t i = 0; i < iNumBone; i++)
	{
		 XMStoreFloat4x4(&m_BoneMatrices[i],
		 	XMLoadFloat4x4(&m_OffsetMatrices[i]) *
		 	pBones[m_BoneIndices[i]]->Get_CombinedTransformMatrix());
	}
	
	return pShader->Bind_Matrices(
		pConstantName,
		m_BoneMatrices,
		g_iMaxNumBones);
}


CSEMesh* CSEMesh::Create_Mesh(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	SE_MESH& seMesh, vector<CSEBone*>& pBones)
{
	CSEMesh* pMesh = new CSEMesh(pDevice, pContext);
	if (FAILED(pMesh->Initialize_Mesh(seMesh, pBones)))
	{
		MSG_BOX("ErrCode : CSEMesh Create Failed");
		Safe_Release(pMesh);
		return nullptr;
	}
	return pMesh;
}

CSEMesh* CSEMesh::Create_AnimMesh(
	ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	SE_ANIM_MESH& seAnimMesh, vector<CSEBone*>& pBones)
{
	CSEMesh* pMesh = new CSEMesh(pDevice, pContext);
	if (FAILED(pMesh->Initialize_AnimMesh(seAnimMesh, pBones)))
	{
		MSG_BOX("ErrCode : CSEAnimMesh Create Failed");
		Safe_Release(pMesh);
		return nullptr;
	}
	return pMesh;
}

CObject* CSEMesh::Clone(void* pArg)
{
	return nullptr;
}

void CSEMesh::Free()
{
	CVIBufferCom::Free();
}

