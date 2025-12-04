#include "SEModel.h"
#include "SEBone.h"
#include "SEMesh.h"
#include "SEMaterial.h"
#include "SEAnimation.h"
#include "AnimationPlayer.h"

#define ANIM_MODEL_PATH "../../SharedProjects/GAME/Asset/Anim_Models/"
#define NONE_ANIM_MODEL_PATH "../../SharedProjects/GAME/Asset/None_Anim_Models/"
#define ANIMATION_FILE_PATH "../../SharedProjects/GAME/Asset/Animations/"

CSEModel::CSEModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CSEModel::CSEModel(const CSEModel& rhs)
	: CComponent(rhs)
	, m_strSaveDataName{ rhs.m_strSaveDataName }
	, m_eAnimType{ rhs.m_eAnimType }
	, m_iNumBone{ rhs.m_iNumBone }
	, m_seBones{ rhs.m_seBones }
	, m_iNumMesh{ rhs.m_iNumMesh }
	, m_seMeshes{ rhs.m_seMeshes }
	, m_seAnimMeshes{ rhs.m_seAnimMeshes }
	, m_iNumMaterial{ rhs.m_iNumMaterial }
	, m_seMaterials{ rhs.m_seMaterials }
	// , m_pBones{ rhs.m_pBones }         
	, m_pMeshes{ rhs.m_pMeshes }       
	, m_pMaterials{ rhs.m_pMaterials }
	// , m_pAnimPlayer(rhs.m_pAnimPlayer)
	, m_InitMatrix{rhs.m_InitMatrix }
{
	for (auto& pBone : rhs.m_pBones)
		m_pBones.push_back(pBone->Clone());

	for (auto& pMesh : m_pMeshes)
		Safe_AddRef(pMesh);

	for (auto& pMat : m_pMaterials)
		Safe_AddRef(pMat);
	if (m_eAnimType == ANIM_TYPE::ANIM)
	{
		CAnimationPlayer::ANIMATIONPLAYER_DESC AnimationPlayerDesc = {};
		AnimationPlayerDesc.m_pBones = &m_pBones;
		m_pAnimPlayer = static_cast<CAnimationPlayer*>(rhs.m_pAnimPlayer->Clone(&AnimationPlayerDesc));
	}
}

HRESULT CSEModel::Initialize_SeModel(
	ANIM_TYPE eAnimType,
	const _char* SaveDataName,
	FXMMATRIX InitMatrix,
	const _string& strInitAnimTag)
{
	m_strSaveDataName = SaveDataName;
	m_eAnimType = eAnimType;
	if (m_eAnimType == ANIM_TYPE::ANIM)
	{
		m_strSaveFilePath = ANIM_MODEL_PATH;
	}
	else if (m_eAnimType == ANIM_TYPE::NONEANIM)
	{
		m_strSaveFilePath = NONE_ANIM_MODEL_PATH;
	}

	XMStoreFloat4x4(&m_InitMatrix, InitMatrix);
	if (FAILED(Load_BoneData()))
		return E_FAIL;

	if (m_eAnimType == ANIM_TYPE::NONEANIM)
	{
		if(FAILED(Load_MeshData()))
			return E_FAIL;
	}
	else if (m_eAnimType == ANIM_TYPE::ANIM)
	{
		if(FAILED(Load_AnimMeshData()))
			return E_FAIL;
	}

	if(FAILED(Load_MaterialData()))
		return E_FAIL;

	if(FAILED(Ready_seBone()))
		return E_FAIL;
	if(FAILED(Ready_seMesh()))
		return E_FAIL;
	if(FAILED(Ready_seMaterial()))
		return E_FAIL;

	if (m_eAnimType == ANIM_TYPE::ANIM)
	{
		m_pAnimPlayer = CAnimationPlayer::Create(m_pDevice, m_pContext, strInitAnimTag , &m_pBones);
	}
	return S_OK;
}


HRESULT CSEModel::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CSEModel::Render_SeModel(uint32_t iMeshIndex)
{
	if (FAILED(m_pMeshes[iMeshIndex]->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pMeshes[iMeshIndex]->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSEModel::Load_BoneData()
{
	_char file[MAX_PATH] = {};
	strcpy_s(file, MAX_PATH, m_strSaveFilePath.c_str());
	strcat_s(file, MAX_PATH, m_strSaveDataName.c_str());
	strcat_s(file, MAX_PATH, "_Bone.bin");

	ifstream in(file, std::ios::binary );

	if (!in.is_open())
		return E_FAIL;

	uint32_t iNumBone = {};
	in.read(reinterpret_cast<char*>(&iNumBone), sizeof(uint32_t));
	m_seBones.resize(iNumBone);
	for (uint32_t i = 0; i < iNumBone; ++i)
	{
		m_seBones[i].ReadSEBoneFile(in);
	}

	in.close();

	m_iNumBone = static_cast<uint32_t>(m_seBones.size());
	return S_OK;
}

HRESULT CSEModel::Load_MeshData()
{
	_char file[MAX_PATH] = {};
	strcpy_s(file, MAX_PATH, m_strSaveFilePath.c_str());
	strcat_s(file, MAX_PATH, m_strSaveDataName.c_str());
	strcat_s(file, MAX_PATH, "_Mesh.bin");

	ifstream in(file, std::ios::binary );

	if (!in.is_open())
		return E_FAIL;

	uint32_t iNumMesh = {};
	in.read(reinterpret_cast<char*>(&iNumMesh), sizeof(uint32_t));
	m_seMeshes.resize(iNumMesh);
	for (uint32_t i = 0; i < iNumMesh; ++i)
	{
		m_seMeshes[i].ReadSEMeshFile(in);
	}

	in.close();

	m_iNumMesh = static_cast<uint32_t>(m_seMeshes.size());
	return S_OK;
}

HRESULT CSEModel::Load_AnimMeshData()
{
	_char file[MAX_PATH] = {};
	strcpy_s(file, MAX_PATH, m_strSaveFilePath.c_str());
	strcat_s(file, MAX_PATH, m_strSaveDataName.c_str());
	strcat_s(file, MAX_PATH, "_AnimMesh.bin");

	ifstream in(file, std::ios::binary);

	if (!in.is_open())
		return E_FAIL;

	uint32_t iNumMesh = {};
	in.read(reinterpret_cast<char*>(&iNumMesh), sizeof(uint32_t));
	m_seAnimMeshes.resize(iNumMesh);
	for (uint32_t i = 0; i < iNumMesh; ++i)
	{
		m_seAnimMeshes[i].ReadSEAnimMeshFile(in);
	}

	in.close();

	m_iNumMesh = static_cast<uint32_t>(m_seAnimMeshes.size());
	return S_OK;
}

HRESULT CSEModel::Load_MaterialData()
{
	_char file[MAX_PATH] = {};
	strcpy_s(file, MAX_PATH, m_strSaveFilePath.c_str());
	strcat_s(file, MAX_PATH, m_strSaveDataName.c_str());
	strcat_s(file, MAX_PATH, "_Material.bin");

	ifstream in(file, std::ios::binary);

	if (!in.is_open())
		return E_FAIL;

	uint32_t iNumMaterial = {};
	in.read(reinterpret_cast<char*>(&iNumMaterial), sizeof(uint32_t));
	m_seMaterials.resize(iNumMaterial);
	for (uint32_t i = 0; i < iNumMaterial; i++)
	{
		m_seMaterials[i].ReadSEMaterialFile(in);
	}
	in.close();

	m_iNumMaterial = static_cast<uint32_t>(m_seMaterials.size());
	return S_OK;
}

HRESULT CSEModel::Ready_seBone()
{
	uint32_t iNumBone = static_cast<uint32_t>(m_seBones.size());

	for (uint32_t i = 0; i < iNumBone; ++i)
	{
		CSEBone* pBone = CSEBone::Create(m_seBones[i]);
		if (nullptr == pBone)
			return E_FAIL;
		m_pBones.push_back(pBone);
	}
	return S_OK;
}

HRESULT CSEModel::Ready_seMesh()
{
	if (m_eAnimType == ANIM_TYPE::NONEANIM)
	{
		uint32_t iNumMesh = static_cast<uint32_t>(m_seMeshes.size());

		for (uint32_t i = 0; i < iNumMesh; ++i)
		{
			CSEMesh* pMesh = CSEMesh::Create_Mesh(
				m_pDevice, m_pContext,
				m_seMeshes[i], m_pBones);
			if (nullptr == pMesh)
				return E_FAIL;
			m_pMeshes.push_back(pMesh);
		}
	}
	else if (m_eAnimType == ANIM_TYPE::ANIM)
	{
		uint32_t iNumMesh = static_cast<uint32_t>(m_seAnimMeshes.size());

		for (uint32_t i = 0; i < iNumMesh; ++i)
		{
			CSEMesh* pMesh = CSEMesh::Create_AnimMesh(
				m_pDevice, m_pContext,
				m_seAnimMeshes[i], m_pBones);
			if (nullptr == pMesh)
				return E_FAIL;
			m_pMeshes.push_back(pMesh);
		}
	}
	return S_OK;
}

HRESULT CSEModel::Ready_seMaterial()
{
	uint32_t iNumMaterial = static_cast<uint32_t>(m_seMaterials.size());

	for (uint32_t i = 0; i < iNumMaterial; ++i)
	{
		CSEMaterial* pMaterial = CSEMaterial::Create(m_pDevice,m_pContext, m_seMaterials[i]);
		if (nullptr == pMaterial)
			return E_FAIL;
		m_pMaterials.push_back(pMaterial);
	}
	return S_OK;
}

HRESULT CSEModel::Bind_Material(
	CShaderCom* pShader, 
	const _char* pConstantName, 
	uint32_t iMeshIndex,
	SE_TEXTURE_TYPE eMaterialType, 
	uint32_t iTextureIndex)
{
	if (iMeshIndex >= m_iNumMesh)
		return E_FAIL;

	_uint       iMaterialIndex = m_pMeshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterial)
		return E_FAIL;

	return m_pMaterials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eMaterialType, iTextureIndex);
}

HRESULT CSEModel::Bind_Bones(
	CShaderCom* pShader,
	const _char* pConstantName,
	uint32_t iMeshIndex)
{
	if (iMeshIndex >= m_iNumMesh)
		return E_FAIL;

	return m_pMeshes[iMeshIndex]->Bind_Bones(pShader, pConstantName, m_pBones);
}

void CSEModel::Change_Animation(const _string strAnimTag, _float fBlendDuration, _bool isLoop )
{
	m_pAnimPlayer->Change_Animation(strAnimTag, fBlendDuration, isLoop);
}

void CSEModel::Play_Animation(_float fDT)
{
	m_pAnimPlayer->Play_Animation(fDT);

	for (auto& pBone : m_pBones)
	{
		pBone->Update_CombinedTransformMatrix(m_pBones, XMLoadFloat4x4(&m_InitMatrix));
	}
}

const XMFLOAT4X4* CSEModel::Get_BoneCombinedTransformMatrixPtr(const _char* pBoneName) const
{
	for (auto& pBone : m_pBones)
	{
		if (pBone->Compare_Name(pBoneName))
			return pBone->Get_CombinedTransformMatrixPtr();
	}

	MSG_BOX("ErrCode : No Bone");
	return nullptr;
}

CSEModel* CSEModel::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	ANIM_TYPE eAnimType,
	const _char* SaveDataName,
	FXMMATRIX InitTransformMatrix,
	const _string& strInitAnimTag)
{
	CSEModel* pInstance = new CSEModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_SeModel(eAnimType, SaveDataName, InitTransformMatrix, strInitAnimTag)))
	{
		MSG_BOX("ErrCode : SeModel Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEModel::Clone(void* pArg)
{
	CSEModel* pInstance = new CSEModel(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : SeModel Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEModel::Free()
{
	CComponent::Free();

	for (auto& pBone : m_pBones)
		Safe_Release(pBone);

	for (auto& pMesh : m_pMeshes)
		Safe_Release(pMesh);

	for (auto& pMat : m_pMaterials)
		Safe_Release(pMat);

	Safe_Release(m_pAnimPlayer);
}
