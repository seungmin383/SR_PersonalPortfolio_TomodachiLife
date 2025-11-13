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
				uint32_t PathSize = static_cast<uint32_t>(TexturePaths[i][j].size());
				Out.write(reinterpret_cast<const char*>(&PathSize), sizeof(uint32_t));
				Out.write(TexturePaths[i][j].c_str(), sizeof(char) * PathSize);
			}
		}
	}

	void ReadSEMaterialFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(int32_t));

		for (uint32_t i = 0; i < 27; ++i)
		{
			uint32_t NumTextures = {};
			In.read(reinterpret_cast<char*>(&NumTextures), sizeof(uint32_t));
			TexturePaths[i].resize(NumTextures);

			for (uint32_t j = 0; j < NumTextures; ++j)
			{
				uint32_t PathSize = {};
				In.read(reinterpret_cast<char*>(&PathSize), sizeof(uint32_t));
				TexturePaths[i][j].resize(PathSize);
				In.read(TexturePaths[i][j].data(), PathSize);
			}
		}
	}


}SE_MATERIAL;
