#pragma once
#include "Engine_Define.h"

typedef struct tagSEBone
{
	string strName;
	uint32_t iIndex;
	int32_t iParentIndex;
	XMFLOAT4X4 TransformMatrix;
	XMFLOAT4X4 OffsetMatrix;

	void WriteBoneFile(ofstream& Out)
	{
		uint32_t NameSize = static_cast<uint32_t>(strName.size());
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(int32_t));
		Out.write(reinterpret_cast<const char*>(&iParentIndex), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&TransformMatrix), sizeof(XMFLOAT4X4));
		Out.write(reinterpret_cast<const char*>(&OffsetMatrix), sizeof(XMFLOAT4X4));
	}

	void ReadSEBoneFile(ifstream& In)
	{
		uint32_t NameSize = {};
		In.read(reinterpret_cast<char*>(&NameSize), sizeof(uint32_t));

		strName.resize(NameSize);
		In.read(strName.data(), NameSize);

		In.read(reinterpret_cast<char*>(&iIndex), sizeof(int32_t));
		In.read(reinterpret_cast<char*>(&iParentIndex), sizeof(uint32_t));
		In.read(reinterpret_cast<char*>(&TransformMatrix), sizeof(XMFLOAT4X4));
		In.read(reinterpret_cast<char*>(&OffsetMatrix), sizeof(XMFLOAT4X4));
	}

}SE_BONE;
