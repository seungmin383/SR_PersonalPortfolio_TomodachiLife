#include "SEMesh.h"

CSEMesh::CSEMesh()
{
}

void CSEMesh::ReadSEMeshFile(ifstream& In)
{
	uint32_t NameSize = {};
	In.read(reinterpret_cast<char*>(&NameSize), sizeof(uint32_t));

	m_strName.resize(NameSize);
	In.read(m_strName.data(), NameSize);

	In.read(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(int32_t));

	uint32_t NumBoneIndices = {};
	In.read(reinterpret_cast<char*>(&NumBoneIndices), sizeof(uint32_t));
	m_BoneIndices.resize(NumBoneIndices);
	In.read(reinterpret_cast<char*>(&m_BoneIndices[0]), sizeof(int32_t) * NumBoneIndices);

	uint32_t NumVertices= {};
	In.read(reinterpret_cast<char*>(&NumVertices), sizeof(uint32_t));
	m_Vertices.resize(NumVertices);
	In.read(reinterpret_cast<char*>(&m_Vertices[0]), sizeof(VTX_TEX_MESH) * NumVertices);

	uint32_t NumIndices = {};
	In.read(reinterpret_cast<char*>(&NumIndices), sizeof(uint32_t));
	mIndices.resize(NumIndices);
	In.read(reinterpret_cast<char*>(&mIndices[0]), sizeof(int32_t) * NumIndices);

}

CSEMesh* CSEMesh::Create()
{
	return new CSEMesh;
}

void CSEMesh::Free()
{
	CBase::Free();
}
