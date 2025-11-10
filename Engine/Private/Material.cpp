#include "Material.h"
#include  "ShaderCom.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize_Origin(
	nlohmann::json& MatData,
	int32_t iMatIndex,
	const aiMaterial* pAIMaterial, 
	const _char* pModelFilePath)
{
	nlohmann::json& jMaterials = MatData["MaterialData"];
	nlohmann::json jMaterial = nlohmann::json::object();

	jMaterial["MaterialIndex"] = (iMatIndex);
	jMaterial["MaterialType"] = nlohmann::json::array();

	nlohmann::json jMaterialType = nlohmann::json::object();
	jMaterialType["TextureType"];
	jMaterialType["NumTexture"];
	jMaterialType["TextureFilePath"] = nlohmann::json::array();

	for (uint32_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		jMaterialType["TextureType"] = i;

		_char szTextureFilePath[MAX_PATH] = {};
		_char szDrivePath[MAX_PATH] = {};
		_char szDirPath[MAX_PATH] = {};

		_splitpath_s(
			pModelFilePath, 
			szDrivePath, MAX_PATH, 
			szDirPath, MAX_PATH, 
			nullptr, 0, 
			nullptr, 0);
	
		uint32_t iNumTexures =
			pAIMaterial->GetTextureCount(
				static_cast<aiTextureType>(i));
		jMaterialType["NumTexture"] = iNumTexures;

		for (uint32_t j = 0; j < iNumTexures; j++)
		{
			aiString strTextureFilePath = {};

			if (aiReturn_SUCCESS != 
				(pAIMaterial->GetTexture(
					static_cast<aiTextureType>(i), 
					j, 
					&strTextureFilePath)))
				return E_FAIL;

			_string TextureFilePath = strTextureFilePath.C_Str();
			jMaterialType["TextureFilePath"].push_back(TextureFilePath);

			_char szFileName[MAX_PATH] = {};
			_char szExt[MAX_PATH] = {};

			strcpy_s(szTextureFilePath, szDrivePath);
			strcat_s(szTextureFilePath, szDirPath);

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
			strcat_s(szTextureFilePath, szFileName);
			strcat_s(szTextureFilePath, szExt);

			_tchar		szFinalPath[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, szTextureFilePath, static_cast<int32_t>(strlen(szTextureFilePath)), szFinalPath, MAX_PATH);

			HRESULT         hr = {};
			ID3D11ShaderResourceView* pSRV = { nullptr };

			if (false == strcmp(szExt, ".dds"))
				hr = CreateDDSTextureFromFile(
					m_pDevice, szFinalPath, nullptr, &pSRV);
			else if (false == strcmp(szExt, ".tga"))
				return E_FAIL;
			else
				hr = CreateWICTextureFromFile(m_pDevice, szFinalPath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_SRVs[i].push_back(pSRV);
		}
		jMaterial["MaterialType"].push_back(jMaterialType);
	}

	jMaterials[iMatIndex] = std::move(jMaterial);
	return S_OK;
}

HRESULT CMaterial::Bind_ShaderResource(CShaderCom* pShader, const _char* pConstantName, aiTextureType eMaterialType, uint32_t iTextureIndex)
{
	if (m_SRVs[eMaterialType].empty())
		return E_FAIL;

	return pShader->Bind_ShaderResource(pConstantName, m_SRVs[eMaterialType][iTextureIndex]);
}

CMaterial* CMaterial::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	nlohmann::json& MatData,
	int32_t iMatDataIndex,
	const aiMaterial* pAIMaterial,
	const _char* pModelFilePath)
{
	CMaterial* pMaterial = new CMaterial(pDevice, pContext);

	if (FAILED(pMaterial->Initialize_Origin(
		MatData,
		iMatDataIndex,
		pAIMaterial, 
		pModelFilePath)))
	{
		Safe_Release(pMaterial);
		MSG_BOX("ErrCode : Material Initialize Failed");
		return nullptr;
	}

	return pMaterial;
}

void CMaterial::Free()
{
	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		for (auto& SRV : m_SRVs[i])
			Safe_Release(SRV);
		m_SRVs[i].clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CBase::Free();
}
