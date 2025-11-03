#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CMesh;
class CMaterial;
class CShaderCom;

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Origin(
		const _char* pModelFilePath,
		FXMMATRIX InitTransformMatrix);
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual HRESULT Render(uint32_t iMeshIndex);

public:
	HRESULT Bind_Material(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iMeshIndex,
		aiTextureType eMaterialType,
		uint32_t iTextureIndex = 0);

public:
	uint32_t Get_NumMesh() { return m_iNumMesh; }
	void Get_Meshes_Vertices(_wstring MeshName, vector<_float2>& Out);

private:
	Assimp::Importer m_Importer = {};
	const aiScene* m_pAIScene = { nullptr };
	
	uint32_t m_iNumMesh = {};
	vector<CMesh*> m_Meshes;
	XMFLOAT4X4 m_InitMatrix = {};

	uint32_t m_iNumMaterial = {};
	vector<CMaterial*> m_Materials;

private:
	HRESULT Ready_Meshes();
	HRESULT	Ready_Material(const _char* pModelFIlePath);

public:
	static CModel* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const _char* pModelFIlePath,
		FXMMATRIX InitTransformMatrix);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END