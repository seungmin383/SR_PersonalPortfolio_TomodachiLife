#pragma once
#include "Engine_Define.h"

typedef struct tagSEMeshData
{
	string strName;
	uint32_t iMaterialIndex;
	vector<int32_t> BoneIndices;
	vector<VTX_TEX_MESH> Vertices;
	vector<int32_t> Indices;

	void WriteMeshFile(ofstream& Out)
	{
		uint32_t NameSize = strName.size();
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&iMaterialIndex), sizeof(uint32_t));

		uint32_t NumBoneIndicies = static_cast<uint32_t>(BoneIndices.size());
		Out.write(reinterpret_cast<const char*>(&NumBoneIndicies), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&BoneIndices[0]), sizeof(int32_t) * NumBoneIndicies);

		uint32_t NumVertices = static_cast<uint32_t>(Vertices.size());
		Out.write(reinterpret_cast<const char*>(&NumVertices), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&Vertices[0]), sizeof(VTX_TEX_MESH) * NumVertices);

		uint32_t NumIndicies = static_cast<uint32_t>(Indices.size());
		Out.write(reinterpret_cast<const char*>(&NumIndicies), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&Indices[0]), sizeof(int32_t) * NumIndicies);
	}

}SE_MESH;

typedef struct tagSEAnimMeshData
{
	string strName;
	int32_t iMaterialIndex;
	vector<int32_t> BoneIndices;
	vector<VTX_TEX_ANIM_MESH> Vertices;
	vector<int32_t> Indices;
	
	void WriteAnimMeshFile(ofstream& Out)
	{
		uint32_t NameSize = strName.size();
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&iMaterialIndex), sizeof(int32_t));

		uint32_t NumBoneIndicies = static_cast<uint32_t>(BoneIndices.size());
		Out.write(reinterpret_cast<const char*>(&NumBoneIndicies), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&BoneIndices[0]), sizeof(int32_t) * NumBoneIndicies);

		uint32_t NumVertices = static_cast<uint32_t>(Vertices.size());
		Out.write(reinterpret_cast<const char*>(&NumVertices), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&Vertices[0]), sizeof(VTX_TEX_ANIM_MESH) * NumVertices);

		uint32_t NumIndicies = static_cast<uint32_t>(Indices.size());
		Out.write(reinterpret_cast<const char*>(&NumIndicies), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&Indices[0]), sizeof(int32_t) * NumIndicies);
	}

}SE_ANIM_MESH;