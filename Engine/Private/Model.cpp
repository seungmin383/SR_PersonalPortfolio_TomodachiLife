#include "GameInstance.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Bone.h"
#include "Animation.h"

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
	m_Materials(rhs.m_Materials),
	m_eAnimType(rhs.m_eAnimType),
	m_Bones(rhs.m_Bones),
	m_Animations(rhs.m_Animations)
{
	for (auto& mesh : m_Meshes)
		Safe_AddRef(mesh);

	for (auto& material : m_Materials)
		Safe_AddRef(material);

	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);

	for (auto& pAnim : m_Animations)
		Safe_AddRef(pAnim);
}

HRESULT CModel::Initialize_Origin(
	_string strProtoTag,
	ANIM_TYPE eAnimType,
	const _char* pModelFilePath,
	FXMMATRIX InitTransformMatrix)
{
	m_pGameInstance->FileManager_Read_File(
		MODEL_DATA_PATH, m_ModelData);

	m_pGameInstance->FileManager_RoundFloatsInJson(m_ModelData, 6);
	/* ProtoTag가 있다면 m_isFirstIntantiated 를 FALSE로, 없다면 TRUE로*/
	Ready_Model(m_ModelData["Model"], strProtoTag, eAnimType);

	uint32_t iFlag ={
		aiProcess_ConvertToLeftHanded | 
		aiProcessPreset_TargetRealtime_Fast };

	if (ANIM_TYPE::NONEANIM == m_eAnimType)
		iFlag |= aiProcess_PreTransformVertices;
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
	{
		MSG_BOX("ErrCode : 긴장해라.");
		return E_FAIL;
	}

	XMStoreFloat4x4(&m_InitMatrix, InitTransformMatrix);

	auto& jModelArr = m_ModelData["Model"];
	auto& jCurModelData = jModelArr.at(m_iCurModelDataIndex);
	
	if (true)//m_isFirstInstantiaed)
	{
		nlohmann::json jBoneData = nlohmann::json::array();

		if (FAILED(Ready_Bones(
			jBoneData,
			m_pAIScene->mRootNode,
			-1)))
			return E_FAIL;

		jCurModelData["BoneData"].push_back(jBoneData);
	}
	else
	{
	}

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Material(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	//m_pGameInstance->FileManager_Write_File(
	//	MODEL_DATA_PATH,
	//	m_ModelData);

	uint32_t i = m_pAIScene->mNumAnimations;
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

HRESULT CModel::Bind_Material(
	CShaderCom* pShader,
	const _char* pConstantName, 
	uint32_t iMeshIndex, 
	aiTextureType eMaterialType, 
	uint32_t iTextureIndex)
{
	if (iMeshIndex >= m_iNumMesh)
		return E_FAIL;

	_uint       iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterial)
		return E_FAIL;


	return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eMaterialType, iTextureIndex);
}

HRESULT CModel::Bind_Bones(
	CShaderCom* pShader, 
	const _char* pConstantName, 
	uint32_t iMeshIndex)
{
	if (iMeshIndex >= m_iNumMesh)
		return E_FAIL;

	return m_Meshes[iMeshIndex]->Bind_Bones(pShader, pConstantName, m_Bones);
}

int32_t CModel::Get_BoneIndex(const _char* pBoneName) const
{
	int32_t iIndex = { 0 };

	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (pBone->Compare_Name(pBoneName))
				return true;

			++iIndex;

			return false;
		});

	if (iter == m_Bones.end())
		return -1;

	return iIndex;
}

void CModel::Play_Animation(_float fDT)
{
	m_Animations[m_iCurAnimationIndex]->Update_TransformationMatrices(
		m_Bones, fDT, m_isAnimLoop);

	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(
			m_Bones,
			XMLoadFloat4x4(&m_InitMatrix));
	}
}

void CModel::Set_Animation(uint32_t iAnimIndex, _bool isLoop)
{
	m_iCurAnimationIndex = iAnimIndex;
	m_isAnimLoop = isLoop;
}

HRESULT CModel::Ready_Meshes()
{
	// m_iNumMesh = (m_ModelData["Model"][m_iCurModelDataIndex]["MeshData"].size());
	m_iNumMesh = m_pAIScene->mNumMeshes;

	auto& jModelArr = m_ModelData["Model"];
	auto& jCurModel = jModelArr.at(m_iCurModelDataIndex);
	nlohmann::json jMeshData = nlohmann::json::array();

	for (uint32_t i = 0; i < m_iNumMesh; i++)
	{
		m_pAIScene->mMeshes[i]->mName;

		CMesh* pMesh = 
			CMesh::Create(
				m_pDevice,
				m_pContext,
				jMeshData,
				i,
				m_eAnimType,
				this,
				m_pAIScene->mMeshes[i],
				XMLoadFloat4x4(&m_InitMatrix));

		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	jCurModel["MeshData"].push_back(jMeshData);
	return S_OK;
}

HRESULT CModel::Ready_Material(const _char* pModelFIlePath)
{
	m_iNumMaterial = m_pAIScene->mNumMaterials;

	for (uint32_t i = 0; i < m_iNumMaterial; i++)
	{
		CMaterial* pMaterial =
			CMaterial::Create(
				m_pDevice,
				m_pContext,
				m_ModelData["Model"][m_iCurModelDataIndex],
				i,
				m_pAIScene->mMaterials[i],
				pModelFIlePath);

		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(
	nlohmann::json& BoneDataArr,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	CBone* pInstance = CBone::Create(
		BoneDataArr,
		m_iCurBoneIndex++,
		pAINode, 
		iParentBoneIndex);

	if (nullptr == pInstance)
		return E_FAIL;

	m_Bones.push_back(pInstance);

	int32_t iParentIndex = static_cast<int32_t>(m_Bones.size() - 1);

	for (uint32_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(
			BoneDataArr,
			pAINode->mChildren[i], 
			iParentIndex);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimation = m_pAIScene->mNumAnimations;

	/* 공격 애니메이션을 만들고 보관, 걷기 애니메이션을 만들고 보관 */
	for (uint32_t i = 0; i < m_iNumAnimation; i++)
	{
		CAnimation* pInstance = CAnimation::Create(
			m_pAIScene->mAnimations[i],
			this);

		if (nullptr == pInstance)
			return E_FAIL;

		m_Animations.push_back(pInstance);
	}
	return S_OK;
}

void CModel::Ready_Model(
	nlohmann::json& InitModelData,
	_string strProtoTag,
	ANIM_TYPE eAnimType)
{
	m_eAnimType = eAnimType;

	int32_t iSize = static_cast<int32_t>(InitModelData.size());

	for (int32_t i = 0; i < iSize; i++)
	{
		if (strProtoTag == InitModelData[i]["ProtoTag"].get<_string>())
		{
			m_iCurModelDataIndex = i;
			m_isFirstInstantiaed = FALSE;
			return;
		}
	}

	nlohmann::json jNewModelData = nlohmann::json::object();

	if (ANIM_TYPE::NONEANIM == eAnimType)
	{
		jNewModelData = { 
			{"ProtoTag", strProtoTag}, 
			{"isAnim", false} };
	}
	else
	{
		jNewModelData = { 
			{"ProtoTag", strProtoTag}, 
			{"isAnim", true} };

	}

	m_iCurModelDataIndex = static_cast<int32_t>(InitModelData.size());
	m_isFirstInstantiaed = TRUE;
	InitModelData.push_back(jNewModelData);
}

CModel* CModel::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext,
	_string strProtoTag,
	ANIM_TYPE eAnimType,
	const _char* pModelFIlePath,
	FXMMATRIX InitTransformMatrix)
{
	CModel* pModel = new CModel(pDevice, pContext);
	
	if (FAILED(pModel->Initialize_Origin(
		strProtoTag,
		eAnimType, 
		pModelFIlePath, 
		InitTransformMatrix)))
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

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& pAnim : m_Animations)
		Safe_Release(pAnim);
	m_Animations.clear();

	m_Importer.FreeScene();

	CComponent::Free();
}
