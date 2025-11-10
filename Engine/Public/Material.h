#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CShaderCom;

class CMaterial final : public CBase	
{
private:
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial() = default;

public:
	virtual HRESULT Initialize_Origin(
		nlohmann::json& MatData,
		int32_t iMatIndex,
		const aiMaterial* pAIMaterial,
		const _char* pModelFilePath);

	HRESULT Bind_ShaderResource(
		CShaderCom* pShader, 
		const _char* pConstantName,
		aiTextureType eMaterialType,
		uint32_t iTextureIndex);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	vector<ID3D11ShaderResourceView*> m_SRVs[AI_TEXTURE_TYPE_MAX];
	     
public:
	static CMaterial* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		nlohmann::json& MatData,
		int32_t iMatDataIndex,
		const aiMaterial* pAIMaterial,
		const _char* pModelFilePath);
	
	virtual void Free()override;
};

NS_END