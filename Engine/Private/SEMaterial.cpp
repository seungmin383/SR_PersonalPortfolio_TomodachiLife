#include "SEMaterial.h"

CSEMaterial::CSEMaterial()
{
}

void CSEMaterial::ReadSEMaterialFile(ifstream& In)
{
	In.read(reinterpret_cast<char*>(&m_iIndex), sizeof(int32_t));

	for (uint32_t i = 0; i < 27; ++i)
	{
		uint32_t iNumTextures = {};
		In.read(reinterpret_cast<char*>(&iNumTextures), sizeof(uint32_t));
		m_TexturePaths[i].resize(iNumTextures);
		
		for (uint32_t j = 0; j < iNumTextures; ++j)
		{
			uint32_t iPathSize = {};
			In.read(reinterpret_cast<char*>(&iPathSize), sizeof(uint32_t));
			m_TexturePaths[i][j].resize(iPathSize);
			In.read(m_TexturePaths[i][j].data(), iPathSize);
		}
	}
}

CSEMaterial* CSEMaterial::Create()
{
	return new CSEMaterial;
}

void CSEMaterial::Free()
{
	CBase::Free();
}
