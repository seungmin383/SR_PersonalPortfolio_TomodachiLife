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
		uint32_t NameSize = static_cast<uint32_t>(strName.size());
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&iMaterialIndex), sizeof(uint32_t));

		 uint32_t NumBoneIndicies = static_cast<uint32_t>(BoneIndices.size());
		 Out.write(reinterpret_cast<const char*>(&NumBoneIndicies), sizeof(uint32_t));
		 if(0 < NumBoneIndicies)
			Out.write(reinterpret_cast<const char*>(BoneIndices.data()), sizeof(int32_t) * NumBoneIndicies);


		uint32_t NumVertices = static_cast<uint32_t>(Vertices.size());
		Out.write(reinterpret_cast<const char*>(&NumVertices), sizeof(uint32_t));
		if (0 < NumVertices)
			Out.write(reinterpret_cast<const char*>(Vertices.data()), sizeof(VTX_TEX_MESH) * NumVertices);

		uint32_t NumIndicies = static_cast<uint32_t>(Indices.size());
		Out.write(reinterpret_cast<const char*>(&NumIndicies), sizeof(uint32_t));
		if(0 < NumIndicies)
			Out.write(reinterpret_cast<const char*>(Indices.data()), sizeof(int32_t) * NumIndicies);
	}

	void ReadSEMeshFile(ifstream& In)
	{
		uint32_t NameSize = {};
		In.read(reinterpret_cast<char*>(&NameSize), sizeof(uint32_t));

		strName.resize(NameSize);
		In.read(strName.data(), NameSize);

		In.read(reinterpret_cast<char*>(&iMaterialIndex), sizeof(uint32_t));

		uint32_t NumBoneIndices = {};
		In.read(reinterpret_cast<char*>(&NumBoneIndices), sizeof(uint32_t));
		if (0 < NumBoneIndices)
		{
			BoneIndices.resize(NumBoneIndices);
			In.read(reinterpret_cast<char*>(BoneIndices.data()), sizeof(int32_t) * NumBoneIndices);
		}

		uint32_t NumVertices = {};
		In.read(reinterpret_cast<char*>(&NumVertices), sizeof(uint32_t));
		if (0 < NumVertices)
		{
			Vertices.resize(NumVertices);
			In.read(reinterpret_cast<char*>(Vertices.data()), sizeof(VTX_TEX_MESH) * NumVertices);
		}

		uint32_t NumIndices = {};
		In.read(reinterpret_cast<char*>(&NumIndices), sizeof(uint32_t));
		if (0 < NumIndices)
		{
			Indices.resize(NumIndices);
			In.read(reinterpret_cast<char*>(Indices.data()), sizeof(int32_t) * NumIndices);
		}

	}
}SE_MESH;

typedef struct tagSEAnimMeshData
{
	string strName;
	uint32_t iMaterialIndex;
	vector<int32_t> BoneIndices;
	vector<VTX_TEX_ANIM_MESH> Vertices;
	vector<int32_t> Indices;
	
	void WriteAnimMeshFile(ofstream& Out)
	{
		uint32_t NameSize = static_cast<uint32_t>(strName.size());
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&iMaterialIndex), sizeof(uint32_t));

		uint32_t NumBoneIndicies = static_cast<uint32_t>(BoneIndices.size());
		Out.write(reinterpret_cast<const char*>(&NumBoneIndicies), sizeof(uint32_t));
		if(0 < NumBoneIndicies)
			Out.write(reinterpret_cast<const char*>(BoneIndices.data()), sizeof(int32_t) * NumBoneIndicies);

		uint32_t NumVertices = static_cast<uint32_t>(Vertices.size());
		Out.write(reinterpret_cast<const char*>(&NumVertices), sizeof(uint32_t));
		if (0 < NumVertices)
			Out.write(reinterpret_cast<const char*>(Vertices.data()), sizeof(VTX_TEX_ANIM_MESH) * NumVertices);

		uint32_t NumIndicies = static_cast<uint32_t>(Indices.size());
		Out.write(reinterpret_cast<const char*>(&NumIndicies), sizeof(uint32_t));
		if (0 < NumIndicies)
			Out.write(reinterpret_cast<const char*>(Indices.data()), sizeof(int32_t) * NumIndicies);
	}

	void ReadSEAnimMeshFile(ifstream& In)
	{
		uint32_t NameSize = {};
		In.read(reinterpret_cast<char*>(&NameSize), sizeof(uint32_t));

		strName.resize(NameSize);
		In.read(strName.data(), NameSize);

		In.read(reinterpret_cast<char*>(&iMaterialIndex), sizeof(uint32_t));

		uint32_t NumBoneIndices = {};
		In.read(reinterpret_cast<char*>(&NumBoneIndices), sizeof(uint32_t));
		if (0 < NumBoneIndices)
		{
			BoneIndices.resize(NumBoneIndices);
			In.read(reinterpret_cast<char*>(BoneIndices.data()), sizeof(int32_t) * NumBoneIndices);
		}

		uint32_t NumVertices = {};
		In.read(reinterpret_cast<char*>(&NumVertices), sizeof(uint32_t));
		if (0 < NumVertices)
		{
			Vertices.resize(NumVertices);
			In.read(reinterpret_cast<char*>(Vertices.data()), sizeof(VTX_TEX_ANIM_MESH) * NumVertices);
		}

		uint32_t NumIndices = {};
		In.read(reinterpret_cast<char*>(&NumIndices), sizeof(uint32_t));
		if (0 < NumIndices)
		{
			Indices.resize(NumIndices);
			In.read(reinterpret_cast<char*>(Indices.data()), sizeof(int32_t) * NumIndices);
		}
	}
}SE_ANIM_MESH;