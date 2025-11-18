#include "SEMaterial.h"
#include "ShaderCom.h"

CSEMaterial::CSEMaterial(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
}

HRESULT CSEMaterial::Initialize(SE_MATERIAL& seMat)
{
	m_iIndex = seMat.iIndex;
	for (uint32_t i = 0; i < 27; i++)
	{
		m_TexturePaths[i] = seMat.TexturePaths[i];
		uint32_t iNumTexture = static_cast<uint32_t>(m_TexturePaths[i].size());
		for (uint32_t j = 0; j < iNumTexture; ++j)
		{
			if (FAILED(Ready_SRVs(i, m_TexturePaths[i][j])))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CSEMaterial::Ready_SRVs(
	uint32_t iTextureType,
	const _string& strTextureFilePath)
{
	_char szExt[MAX_PATH] = {};

	_splitpath_s(strTextureFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	_tchar		szFinalPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0,
		strTextureFilePath.c_str(),
		static_cast<int32_t>(strTextureFilePath.length()),
		szFinalPath, MAX_PATH);

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

	m_SRVs[iTextureType].push_back(pSRV);
	return S_OK;
}

HRESULT CSEMaterial::Bind_ShaderResource(
	CShaderCom* pShader, 
	const _char* pConstantName, 
	SE_TEXTURE_TYPE eMaterialType, 
	uint32_t iTextureIndex)
{
	if (m_SRVs[eMaterialType].empty())
		return E_FAIL;

	return pShader->Bind_ShaderResource(
		pConstantName, m_SRVs[eMaterialType][iTextureIndex]);
}

HRESULT CSEMaterial::Bind_tmpSRV(CShaderCom* pShader)
{
	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC			TextureDesc{};
	TextureDesc.Width				= 1;
	TextureDesc.Height				= 1;
	TextureDesc.MipLevels			= 1;
	TextureDesc.ArraySize			= 1;
	TextureDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality	= 0;
	TextureDesc.SampleDesc.Count	= 1;

	TextureDesc.Usage				= D3D11_USAGE_IMMUTABLE;
	TextureDesc.BindFlags			= 0;

	TextureDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags			= 0;

	_uint* pInitialPixels = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pInitialPixels, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	/* a b g r */
	pInitialPixels[0] = D3DCOLOR_ARGB(255, 255, 255, 255);

	D3D11_SUBRESOURCE_DATA		InitialData{};
	InitialData.pSysMem = pInitialPixels;
	InitialData.SysMemPitch = sizeof(_uint) * TextureDesc.Width;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
	SRVDesc.Format = TextureDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;
	
	return S_OK;
}

CSEMaterial* CSEMaterial::Create(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext, 
	SE_MATERIAL& seMat)
{
	CSEMaterial* pInstance = new CSEMaterial(pDevice, pContext);
	if (FAILED(pInstance->Initialize(seMat)))
	{
		MSG_BOX("ErrCode : CSEMaterial Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEMaterial::Free()
{
	Safe_Release (m_pDevice);
	Safe_Release(m_pContext);

	
	for (auto& pSRVs : m_SRVs)
	{
		for (auto& pSRV : pSRVs)
			Safe_Release(pSRV);
	}

	CBase::Free();
}
