#include"TextureCom.h"
#include "ShaderCom.h"
#define CORRECT 0

CTextureCom::CTextureCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CTextureCom::CTextureCom(const CTextureCom& rhs)
	:CComponent(rhs),
	m_Textures(rhs.m_Textures),
	m_iNumTextures(rhs.m_iNumTextures)
{
	//따로 로딩하지 않고 얕은 복사로
	for(auto& pSRVs : m_Textures)
		Safe_AddRef(pSRVs);
}

HRESULT CTextureCom::Initialize_Origin(const _tchar* pTextureFilePath, uint32_t iNumTextures)
{
	m_iNumTextures = iNumTextures;

	m_Textures.reserve(m_iNumTextures);

	for(uint32_t i = 0; i < m_iNumTextures; ++i)
	{
		_tchar		szTextureFilePath[MAX_PATH] = TEXT(""); //문자열 배열 생성
		ID3D11ShaderResourceView* pSRV = { nullptr };		//리소스뷰 생성

		wsprintf(szTextureFilePath, pTextureFilePath, i);	//pTextureFilePath에 i를 넣어서 szTextureFilePath에 저장

		_tchar		szExt[MAX_PATH] = {};					//확장자 저장할 배열

		_wsplitpath_s(
			szTextureFilePath,
			nullptr, 
			0, 
			nullptr, 
			0, 
			nullptr, 
			0, 
			szExt, 
			MAX_PATH);
		
		//lstrcmp : 두 문자열이 같으면 0, 다르면 0이 아닌 값 반환 -> True
		if (CORRECT == lstrcmp(szExt, TEXT(".dds")))
		{
			if (FAILED(CreateDDSTextureFromFile(
				m_pDevice, 
				szTextureFilePath, 
				nullptr, 
				&pSRV)))		
				return E_FAIL;
		}
		else if (CORRECT == lstrcmp(szExt, TEXT(".tga")))
		{
			return E_FAIL;
		}
		else
		{
			if (FAILED(CreateWICTextureFromFile(
				m_pDevice, 
				szTextureFilePath, 
				nullptr,			//Texture2D
				&pSRV)))
				return E_FAIL;
		}

		m_Textures.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTextureCom::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CTextureCom::Update_Priority(const _float fDT)
{
	return 0;
}

int32_t CTextureCom::Update(const _float fDT)
{
	return 0;
}

void CTextureCom::Update_Late(const _float fDT)
{
}

HRESULT CTextureCom::Render()
{
	return S_OK;
}

HRESULT CTextureCom::Bind_ShaderResource(CShaderCom* pShader, const _char* pConstantName, uint32_t iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_ShaderResource(pConstantName, m_Textures[iTextureIndex]);
}

HRESULT CTextureCom::Bind_ShaderResources(CShaderCom* pShader, const _char* pConstantName)
{
	return pShader->Bind_ShaderResources(
		pConstantName,
		&m_Textures.front(),
		m_iNumTextures);
}

CTextureCom* CTextureCom::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext,
	const _tchar* pTextureFilePath, 
	uint32_t iNumTextures)
{
	CTextureCom* pTexture = new CTextureCom(pDevice, pContext);
	
	if (nullptr == pTexture)
	{
		MSG_BOX("ErrCode : TextureCom Create Failed");
		Safe_Release(pTexture);
		return nullptr;
	}

	if (FAILED(pTexture->Initialize_Origin(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("ErrCode : TextureCom Initializing Failed");
		Safe_Release(pTexture);
		return nullptr;
	}

	return pTexture;
}

CObject* CTextureCom::Clone(void* pArg)
{
	CTextureCom* pTexture = new CTextureCom(*this);

	if (FAILED(pTexture->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pTexture);
	}

	return pTexture;
}

void CTextureCom::Free()
{
	CComponent::Free();

	for (auto& pSRV : m_Textures)
		Safe_Release(pSRV);
	m_Textures.clear();
}
