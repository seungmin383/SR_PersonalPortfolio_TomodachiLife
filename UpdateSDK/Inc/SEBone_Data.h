#pragma once
#include "Engine_Define.h"

typedef struct tagSEBone
{
	string strName;
	int32_t iIndex;
	int32_t iParentIndex;
	XMFLOAT4X4 TransformMatrix;
	XMFLOAT4X4 OffsetMatrix;

	void WriteBoneFile(ofstream& Out)
	{
		uint32_t NameSize = strName.size();
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(int32_t));
		Out.write(reinterpret_cast<const char*>(&iParentIndex), sizeof(int32_t));
		Out.write(reinterpret_cast<const char*>(&TransformMatrix), sizeof(XMFLOAT4X4));
		Out.write(reinterpret_cast<const char*>(&OffsetMatrix), sizeof(XMFLOAT4X4));
	}

}SE_BONE;
