#pragma once
#include "Base.h"
#include "SEMaterial_Data.h"

NS_BEGIN(Engine)
class CShaderCom;

class ENGINE_DLL CSEMaterial final : public CBase
{
private:
	CSEMaterial(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual ~CSEMaterial() = default;

public:
	HRESULT Initialize(SE_MATERIAL& seMat);

	HRESULT Ready_SRVs(
		uint32_t iTextureType,
		const _string& strTextureFilePath);

	HRESULT Bind_ShaderResource(
		CShaderCom* pShader,
		const _char* pConstantName,
		SE_TEXTURE_TYPE eMaterialType,
		uint32_t iTextureIndex);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	int32_t m_iIndex = {};
	vector<string> m_TexturePaths[27];
	vector<ID3D11ShaderResourceView*> m_SRVs[27];

public:
	static CSEMaterial* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		SE_MATERIAL& seMat);
	virtual void Free()override;
};

NS_END