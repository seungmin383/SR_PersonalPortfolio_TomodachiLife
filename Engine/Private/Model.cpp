#include "GameInstance.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Bone.h"
#include "Animation.h"
#include "SEAnimation.h"

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
	FXMMATRIX InitTransformMatrix,
	const _char* pSaveDataName)
{
	uint32_t iFlag ={
		aiProcess_ConvertToLeftHanded | 
		aiProcessPreset_TargetRealtime_Fast };

	m_eAnimType = eAnimType;

	if (ANIM_TYPE::NONEANIM == m_eAnimType)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
	{
		MSG_BOX("ErrCode : 긴장해라.");
		return E_FAIL;
	}

	XMStoreFloat4x4(&m_InitMatrix, InitTransformMatrix);

	BuildOffsetMap();

	if (FAILED(Ready_Bones(
		m_pAIScene->mRootNode,
		-1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Material(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	uint32_t i = m_pAIScene->mNumAnimations;

	Save_Bone(pSaveDataName);
	Save_Mesh(pSaveDataName);
	Save_Material(pSaveDataName);
	

	if (eAnimType == ANIM_TYPE::ANIM)
	{
		_char file[MAX_PATH] = {};

		strcpy_s(file, MAX_PATH, "../Asset/Animations/");
		strcat_s(file, MAX_PATH, m_SEAnimationToWrite[0].strName.c_str());
		strcat_s(file, MAX_PATH, "_Anim.bin");

		ofstream out(file, std::ios::binary | std::ios::trunc);
		m_SEAnimationToWrite[0].WriteAnimationFile(out);
	}
	
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

const XMFLOAT4X4& CModel::Get_OffsetMatrixByName(const _string& name) const
{
	static XMFLOAT4X4 s_identity;
	static bool s_inited = false;
	if (!s_inited) { XMStoreFloat4x4(&s_identity, XMMatrixIdentity()); s_inited = true; }

	auto it = m_OffsetMap.find(name);
	return (it != m_OffsetMap.end()) ? it->second : s_identity;
}

const XMFLOAT4X4* CModel::Get_BoneMatrixPtr(const _char* pBoneName) const
{
	int32_t iBoneIndex = Get_BoneIndex(pBoneName);
	if (-1 == iBoneIndex)
		return nullptr;

	return m_Bones[iBoneIndex]->Get_CombinedTransformationMatrixPtr();
}

void CModel::BuildOffsetMap()
{ 
	m_OffsetMap.clear();

    for (uint32_t i = 0; i < m_pAIScene->mNumMeshes; ++i)
    {
        const aiMesh* pMesh = m_pAIScene->mMeshes[i];
        if (!pMesh)
			continue;

        for (uint32_t j = 0; j < pMesh->mNumBones; ++j)
        {
            const aiBone* pBone = pMesh->mBones[j];
            if (!pBone) 
				continue;

            _string name = pBone->mName.C_Str();
			XMFLOAT4X4 offsetMatrix = {};
			memcpy(&offsetMatrix, &pBone->mOffsetMatrix, sizeof(XMFLOAT4X4));
			XMFLOAT4X4 xm = {};
			XMStoreFloat4x4(&xm, XMMatrixTranspose(XMLoadFloat4x4(&offsetMatrix))); // ※ 전치 필요 시 정책에 따라 여기서 1회만

            auto it = m_OffsetMap.find(name);

            if (it == m_OffsetMap.end())
            {
                m_OffsetMap.emplace(std::move(name), xm);
            }
            else
            {
            }
        }
    }
}

HRESULT CModel::Save_Bone(const _char* pModelName)
{
	_char file[MAX_PATH] = {};

	strcpy_s(file, MAX_PATH, "../Asset/");
	strcat_s(file, MAX_PATH, pModelName);
	strcat_s(file, MAX_PATH, "_Bone_Data.bin");

	ofstream out(file, std::ios::binary | std::ios::trunc);

	uint32_t count = static_cast<uint32_t>(m_SEBonesToWrite.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

	for (uint32_t i = 0; i < count; ++i)
	{
		m_SEBonesToWrite[i].WriteBoneFile(out);
	}
	
	return S_OK;
}

HRESULT CModel::Save_Mesh(const _char* pModelName)
{
	if (m_eAnimType == ANIM_TYPE::NONEANIM)
	{
		_char file[MAX_PATH] = {};

		strcpy_s(file, MAX_PATH, "../Asset/");
		strcat_s(file, MAX_PATH, pModelName);
		strcat_s(file, MAX_PATH, "_Mesh_Data.bin");

		ofstream out(file, std::ios::binary | std::ios::trunc);

		uint32_t count = static_cast<uint32_t>(m_SEMeshesToWrite.size());
		out.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

		for (uint32_t i = 0; i < count; ++i)
		{
			m_SEMeshesToWrite[i].WriteMeshFile(out);
		}

	}
	else if (m_eAnimType == ANIM_TYPE::ANIM)
	{
		_char file[MAX_PATH] = {};

		strcpy_s(file, MAX_PATH, "../Asset/");
		strcat_s(file, MAX_PATH, pModelName);
		strcat_s(file, MAX_PATH, "_AnimMesh_Data.bin");

		ofstream out(file, std::ios::binary | std::ios::trunc);

		uint32_t count = static_cast<uint32_t>(m_SEAnimMeshesToWrite.size());
		out.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

		for (uint32_t i = 0; i < count; ++i)
		{
			m_SEAnimMeshesToWrite[i].WriteAnimMeshFile(out);
		}
	}
	return S_OK;
}

HRESULT CModel::Save_Material(const _char* pModelName)
{
	_char file[MAX_PATH] = {};

	strcpy_s(file, MAX_PATH, "../Asset/");
	strcat_s(file, MAX_PATH, pModelName);
	strcat_s(file, MAX_PATH, "_Mat_Data.bin");

	ofstream out(file, std::ios::binary | std::ios::trunc);

	uint32_t count = static_cast<uint32_t>(m_SEMaterialsToWrite.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

	for (uint32_t i = 0; i < count; ++i)
	{
		m_SEMaterialsToWrite[i].WriteMaterialFile(out);
	}

	return S_OK;
}

HRESULT CModel::Save_Animation(const _char* pAnimationName)
{
	_char file[MAX_PATH] = {};

	strcpy_s(file, MAX_PATH, "../Asset/Animations/");
	strcat_s(file, MAX_PATH, pAnimationName);
	strcat_s(file, MAX_PATH, "_Anim_Data.bin");

	ofstream out(file, std::ios::binary | std::ios::trunc);

	uint32_t count = static_cast<uint32_t>(m_SEAnimationToWrite.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

	for (uint32_t i = 0; i < count; ++i)
	{
		m_SEAnimationToWrite[i].WriteAnimationFile(out);
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMesh = m_pAIScene->mNumMeshes;

	for (uint32_t i = 0; i < m_iNumMesh; i++)
	{
		SE_MESH seMesh = {};
		SE_ANIM_MESH seAnimMesh = {};
		m_pAIScene->mMeshes[i]->mName;

			CMesh* pMesh = 
				CMesh::Create(
					m_pDevice,
					m_pContext,
					seMesh,
					seAnimMesh,
					m_eAnimType,
					this,
					m_pAIScene->mMeshes[i],
					XMLoadFloat4x4(&m_InitMatrix));
	

		if (m_eAnimType == ANIM_TYPE::ANIM)
		{
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
			m_SEAnimMeshesToWrite.push_back(seAnimMesh);

		}
		else if (m_eAnimType == ANIM_TYPE::NONEANIM)
		{
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
			m_SEMeshesToWrite.push_back(seMesh);

		}
	}

	return S_OK;
}


HRESULT CModel::Ready_Material(const _char* pModelFIlePath)
{
	m_iNumMaterial = m_pAIScene->mNumMaterials;

	for (uint32_t i = 0; i < m_iNumMaterial; i++)
	{
		SE_MATERIAL seMat = {};
		CMaterial* pMaterial =
			CMaterial::Create(
				m_pDevice,
				m_pContext,
				seMat,
				m_pAIScene->mMaterials[i],
				pModelFIlePath);

		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
		m_SEMaterialsToWrite.push_back(seMat);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	SE_BONE seBone = {};

	CBone* pInstance = CBone::Create(
		seBone,
		pAINode, 
		iParentBoneIndex);

	if (nullptr == pInstance)
		return E_FAIL;

	m_Bones.push_back(pInstance);

	seBone.OffsetMatrix = Get_OffsetMatrixByName(seBone.strName);
	seBone.iIndex = m_iBoneIndex++;
	m_SEBonesToWrite.push_back(seBone);

	int32_t iParentIndex = static_cast<int32_t>(m_Bones.size() - 1);

	for (uint32_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(
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
		SE_ANIMATION seAnim = {};

		CAnimation* pInstance = CAnimation::Create(
			seAnim,
			m_pAIScene->mAnimations[i],
			this);

		if (nullptr == pInstance)
			return E_FAIL;

		m_Animations.push_back(pInstance);
		m_SEAnimationToWrite.push_back(seAnim);
	}
	return S_OK;
}

CModel* CModel::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext,
	_string strProtoTag,
	ANIM_TYPE eAnimType,
	const _char* pModelFIlePath,
	FXMMATRIX InitTransformMatrix,
	const _char* pSaveDataName)
{
	CModel* pModel = new CModel(pDevice, pContext);
	
	if (FAILED(pModel->Initialize_Origin(
		strProtoTag,
		eAnimType, 
		pModelFIlePath, 
		InitTransformMatrix,
		pSaveDataName)))
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
