#pragma once
#include "Engine_Define.h"

typedef struct tagSEMaterialData
{
	int32_t iIndex;
	vector<string> TexturePaths[27];

	void WriteMaterialFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(int32_t));

		for (uint32_t i = 0; i < 27; i++)
		{
			uint32_t NumTexture = static_cast<uint32_t>(TexturePaths[i].size());
			Out.write(reinterpret_cast<const char*>(&NumTexture), sizeof(uint32_t));

			for (uint32_t j = 0; j < NumTexture; ++j){
				uint32_t iPathSize = static_cast<uint32_t>(TexturePaths[i][j].size());
				Out.write(reinterpret_cast<const char*>(&iPathSize), sizeof(uint32_t));
				Out.write(TexturePaths[i][j].c_str(), sizeof(char) * iPathSize);
			}
		}
	}

	void ReadSEMaterialFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(int32_t));

		for (uint32_t i = 0; i < 27; ++i)
		{
			uint32_t iNumTextures = {};
			In.read(reinterpret_cast<char*>(&iNumTextures), sizeof(uint32_t));
			TexturePaths[i].resize(iNumTextures);

			for (uint32_t j = 0; j < iNumTextures; ++j)
			{
				uint32_t iPathSize = {};
				In.read(reinterpret_cast<char*>(&iPathSize), sizeof(uint32_t));
				TexturePaths[i][j].resize(iPathSize);
				In.read(TexturePaths[i][j].data(), iPathSize);
			}
		}
	}


}SE_MATERIAL;
