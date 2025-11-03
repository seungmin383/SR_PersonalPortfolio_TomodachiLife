#include "Model.h"
#include "Mesh.h"
#include "Material.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs)
	:CComponent(rhs),
	m_iNumMesh(rhs.m_iNumMesh),
	m_Meshes(rhs.m_Meshes),
	m_InitMatrix(rhs.m_InitMatrix),
	m_iNumMaterial(rhs.m_iNumMaterial),
	m_Materials(rhs.m_Materials)
{
	for (auto& mesh : m_Meshes)
		Safe_AddRef(mesh);

	for (auto& material : m_Materials)
		Safe_AddRef(material);
}

HRESULT CModel::Initialize_Origin(
	const _char* pModelFilePath,
	FXMMATRIX InitTransformMatrix)
{
	//aiProcess_PreTransformVertices
	uint32_t iFlag =
	{ aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Quality };

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
	{
		MSG_BOX("ErrCode : 긴장해라.");
		return E_FAIL;
	}

	XMStoreFloat4x4(&m_InitMatrix, InitTransformMatrix);

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Material(pModelFilePath)))
		return E_FAIL;

	uint32_t i = m_pAIScene->mNumAnimations;
	int a = 0;
	return S_OK;
}

HRESULT CModel::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(uint32_t iMeshIndex)
{
	if (FAILED(m_Meshes[iMeshIndex]->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_Meshes[iMeshIndex]->Begin_Draw()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CModel::Bind_Material(CShaderCom* pShader, const _char* pConstantName, uint32_t iMeshIndex, aiTextureType eMaterialType, uint32_t iTextureIndex)
{
	if (iMeshIndex >= m_iNumMesh)
		return E_FAIL;

	_uint       iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterial)
		return E_FAIL;


	return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eMaterialType, iTextureIndex);
}

void CModel::Get_Meshes_Vertices(_wstring MeshName, vector<_float2>& Out)
{

}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMesh = m_pAIScene->mNumMeshes;

	for (uint64_t i = 0; i < m_iNumMesh; i++)
	{
		m_pAIScene->mMeshes[i]->mName;

		CMesh* pMesh = 
			CMesh::Create(
				m_pDevice,
				m_pContext,
				m_pAIScene->mMeshes[i],
				XMLoadFloat4x4(&m_InitMatrix));

		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Material(const _char* pModelFIlePath)
{
	m_iNumMaterial = m_pAIScene->mNumMaterials;

	for (uint64_t i = 0; i < m_iNumMaterial; i++)
	{
		CMaterial* pMaterial =
			CMaterial::Create(
				m_pDevice,
				m_pContext,
				m_pAIScene->mMaterials[i],
				pModelFIlePath);

		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

CModel* CModel::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	const _char* pModelFIlePath,
	FXMMATRIX InitTransformMatrix)
{
	CModel* pModel = new CModel(pDevice, pContext);

	if (FAILED(pModel->Initialize_Origin(pModelFIlePath, InitTransformMatrix)))
	{
		MSG_BOX("ErrCode : Model Initializing Failed");
		Safe_Release(pModel);
		return nullptr;
	}

	return pModel;
}

CObject* CModel::Clone(void* pArg)
{
	CModel* pModel = new CModel(*this);

	if (FAILED(pModel->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Model Clone Failed");
		Safe_Release(pModel);
	}

	return pModel;
}

void CModel::Free()
{
	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);
	m_Materials.clear();

	m_Importer.FreeScene();

	CComponent::Free();
}
