#include "SEBone.h"

CSEBone::CSEBone()
{
}

void CSEBone::ReadSEBoneFile(ifstream& In)
{
	uint32_t NameSize = {};
	In.read(reinterpret_cast<char*>(&NameSize), sizeof(uint32_t));

	m_strName.resize(NameSize);
	In.read(m_strName.data(), NameSize);

	In.read(reinterpret_cast<char*>(&m_iIndex), sizeof(int32_t));
	In.read(reinterpret_cast<char*>(&m_iParentIndex), sizeof(int32_t));
	In.read(reinterpret_cast<char*>(&m_TransformMatrix), sizeof(XMFLOAT4X4));
	In.read(reinterpret_cast<char*>(&m_OffsetMatrix), sizeof(XMFLOAT4X4));
}

CSEBone* CSEBone::Create()
{
	return new CSEBone;
}

void CSEBone::Free()
{
	CBase::Free();
}
