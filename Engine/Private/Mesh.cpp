#include "Mesh.h"
#include "Bone.h"
#include "ShaderCom.h"
#include "Model.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBufferCom(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	:CVIBufferCom(rhs)
{
}

HRESULT CMesh::Initialize_Origin(
	ANIM_TYPE eAnimType,
	CModel* pModel,
	aiMesh* pAIMesh,
	FXMMATRIX InitTransformMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;

	m_iNumVertexBuffers = 1;

	m_iNumVertices = pAIMesh->mNumVertices;						//정점 개수

	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;												//인덱스 하나의 크기

	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	switch (eAnimType)
	{
	case Engine::ANIM_TYPE::ANIM:
		Ready_AnimMesh(pAIMesh, pModel);
		break;
	case Engine::ANIM_TYPE::NONEANIM:
		Ready_NoneAnimMesh(pAIMesh, InitTransformMatrix);
		break;
	default:
		return E_FAIL;
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
	uint64_t IndicesNum = {};

	for (uint64_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		pIndices[IndicesNum++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[IndicesNum++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[IndicesNum++] = pAIMesh->mFaces[i].mIndices[2];
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

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh::Bind_Bones(
	CShaderCom* pShader,
	const _char* pConstantName,
	vector<CBone*>& Bones)
{
	for (uint32_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&m_BoneMatrices[i],
			XMLoadFloat4x4(&m_OffsetMatrices[i]) *
			Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
	}

	return pShader->Bind_Matrices(
		pConstantName,
		m_BoneMatrices,
		g_iMaxNumBones);
}

HRESULT CMesh::Ready_AnimMesh(
	const aiMesh* pAIMesh,
	CModel* pModel)
{
	m_iVertexStride = sizeof(VTX_TEX_ANIM_MESH);							//정점 하나의 크기
	D3D11_BUFFER_DESC VBuffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	VBuffer_Data.Usage = D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	VBuffer_Data.ByteWidth = m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	VBuffer_Data.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	VBuffer_Data.CPUAccessFlags = 0;									//CPU가 접근할 수 있는 권한
	VBuffer_Data.MiscFlags = 0;									//보통 0
	VBuffer_Data.StructureByteStride = m_iVertexStride;						//

	VTX_TEX_ANIM_MESH* pVertices = new VTX_TEX_ANIM_MESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_TEX_ANIM_MESH) * m_iNumVertices);

	for (uint64_t i = 0; i < m_iNumVertices; i++)
	{
		// Position
		memcpy(&pVertices[i].vPos, &pAIMesh->mVertices[i], sizeof(_float3));
		m_pVertexPositions[i] = pVertices[i].vPos;

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexCoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_iNumBones = pAIMesh->mNumBones;

	for (uint32_t i = 0; i < m_iNumBones; i++)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		int32_t iBoneIndex = pModel->Get_BoneIndex(pAIBone->mName.data);

		if (-1 == iBoneIndex)
			return E_FAIL;

		m_BoneIndices.push_back(iBoneIndex);

		XMFLOAT4X4 OffsetMatrix = {};
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(XMFLOAT4X4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		m_OffsetMatrices.push_back(OffsetMatrix);

		for (uint32_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight AIWeight = pAIBone->mWeights[j];

			if (0.f == pVertices[AIWeight.mVertexId].vBlendWeight.x)
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.x = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.x = AIWeight.mWeight;
			}
			else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeight.y)
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.y = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.y = AIWeight.mWeight;
			}
			else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeight.z)
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.z = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.z = AIWeight.mWeight;
			}
			else
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.w = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.w = AIWeight.mWeight;
			}
		}
	}
	
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		XMFLOAT4X4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.push_back(OffsetMatrix);
		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));
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

	Safe_Delete_Array(pVertices);
	return S_OK;
	
}

HRESULT CMesh::Ready_NoneAnimMesh(const aiMesh* pAIMesh, FXMMATRIX InitTransformMatrix)
{
	m_iVertexStride = sizeof(VTX_TEX_MESH);							//정점 하나의 크기
	D3D11_BUFFER_DESC VBuffer_Data = {}; //버텍스 버퍼의 속성을 설정해주는 것

	VBuffer_Data.Usage = D3D11_USAGE_DEFAULT;					//정적으로 할지 동적으로 할지
	VBuffer_Data.ByteWidth = m_iVertexStride * m_iNumVertices;		//버퍼는 배열이므로 데이터를 얼마나 잡아둘까
	VBuffer_Data.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//어떤용도인지
	VBuffer_Data.CPUAccessFlags = 0;									//CPU가 접근할 수 있는 권한
	VBuffer_Data.MiscFlags = 0;									//보통 0
	VBuffer_Data.StructureByteStride = m_iVertexStride;						//

	VTX_TEX_MESH* pVertices = new VTX_TEX_MESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_TEX_MESH) * m_iNumVertices);

	for (uint64_t i = 0; i < m_iNumVertices; i++)
	{
		// Position
		memcpy(&pVertices[i].vPos, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPos, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPos), InitTransformMatrix));

		// Normal 
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), InitTransformMatrix));

		// Tangent
		if (pAIMesh->mTangents == NULL)
			continue;

		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), InitTransformMatrix));

		// Binormal
		memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vBinormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vBinormal), InitTransformMatrix));

		// TexCoord
		memcpy(&pVertices[i].vTexCoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
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

	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CMesh::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CMesh* CMesh::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	ANIM_TYPE eAnimType,
	CModel* pModel,
	aiMesh* pAIMesh,
	FXMMATRIX InitTransformMatrix)
{
	CMesh* pMesh = new CMesh(pDevice, pContext);

	if (FAILED(pMesh->Initialize_Origin(
		eAnimType,
		pModel,
		pAIMesh, 
		InitTransformMatrix)))
	{
		Safe_Release(pMesh);
		MSG_BOX("ErrCode : Mesh Create Failed");
		return nullptr;
	}

	return pMesh;
}

CObject* CMesh::Clone(void* pArg)
{
	CMesh* pMesh = new CMesh(*this);

	if (FAILED(pMesh->Initialize_Clone(pArg)))
	{
		Safe_Release(pMesh);
		MSG_BOX("ErrCode : Mesh Clone Failed");
		return nullptr;
	}

	return pMesh;
}

void CMesh::Free()
{
	CVIBufferCom::Free();
}
