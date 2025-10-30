#include "Model.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs)
	:CComponent(rhs),
	m_iNumMeshes(rhs.m_iNumMeshes),
	m_Meshes(rhs.m_Meshes)
{
	for (auto& mesh : m_Meshes)
		Safe_AddRef(mesh);
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

	uint32_t i = m_pAIScene->mNumAnimations;
	int a = 0;
	return S_OK;
}

HRESULT CModel::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render()
{
	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		m_Meshes[i]->Bind_Resource();
		m_Meshes[i]->Begin_Draw();
	}
	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (uint64_t i = 0; i < m_iNumMeshes; i++)
	{
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

	m_Importer.FreeScene();

	CComponent::Free();
}
