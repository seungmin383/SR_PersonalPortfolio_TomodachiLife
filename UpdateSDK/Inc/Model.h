#pragma once
#include "Component.h"
#include "SEBone_Data.h"
#include "SEMaterial_Data.h"
#include "SEMesh_Data.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)

class CMesh;
class CMaterial;
class CShaderCom;
class CBone;
class CAnimation;

class CSEBone;
class CSEMaterial;
class CSEMesh;
class CSEAnimation;

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Origin(
		ANIM_TYPE eAnimType,
		const _char* pModelFilePath,
		FXMMATRIX InitTransformMatrix,
		const _char* pSaveDataName);

	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual HRESULT Render(uint32_t iMeshIndex);

public:
	HRESULT Bind_Material(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iMeshIndex,
		aiTextureType eMaterialType,
		uint32_t iTextureIndex = 0);

	HRESULT Bind_Bones(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iMeshIndex);

public:
	const XMFLOAT4X4& Get_OffsetMatrixByName(const _string& name) const;
	const XMFLOAT4X4* Get_BoneMatrixPtr(const _char* pBoneName)const;
	const int32_t Get_BoneIndex(const _char* pBoneName)const;
	const uint32_t Get_NumMesh()const{ return m_iNumMesh; }

private:
	void BuildOffsetMap();

	HRESULT Ready_Meshes();
	HRESULT	Ready_Material(const _char* pModelFIlePath);
	HRESULT Ready_Bones(const aiNode* pAINode, int32_t iParentBoneIndex);
	HRESULT Ready_Animations();

	HRESULT Save_Bone(const _char* pModelName);
	HRESULT Save_Mesh(const _char* pModelName);
	HRESULT Save_Material(const _char* pModelName);
	HRESULT Save_Animation(const _char* pAnimationName);

private:
	Assimp::Importer		m_Importer = {};
	const aiScene*			m_pAIScene = { nullptr };

	_string					m_strSaveFilePath = {};
	XMFLOAT4X4				m_InitMatrix = {};
	ANIM_TYPE				m_eAnimType = { ANIM_TYPE::END };

	vector<SE_BONE>			m_SEBonesToWrite;
	vector<SE_MATERIAL>		m_SEMaterialsToWrite;
	vector<SE_MESH>			m_SEMeshesToWrite;
	vector<SE_ANIM_MESH>	m_SEAnimMeshesToWrite;
	vector<SE_ANIMATION>	m_SEAnimationToWrite;

	vector<CSEBone*>		m_SEBoneToRead;
	vector<CSEMaterial*>	m_SEMaterialToRead;
	vector<CSEMesh*>		m_SEMeshToRead;
	vector<CSEAnimation*>	m_SEAnimationToRead;

	uint32_t				m_iNumMesh = {};
	vector<CMesh*>			m_Meshes;

	uint32_t				m_iNumMaterial = {};
	vector<CMaterial*>		m_Materials;

	vector<CBone*>			m_Bones;
	uint32_t				m_iBoneIndex = {};

	uint32_t				m_iNumAnimation = {};
	uint32_t				m_iCurAnimationIndex = {};

	vector<CAnimation* >	m_Animations;
	_bool					m_isAnimLoop = { FALSE };
	_bool					m_isAnimFinished = { FALSE };

	unordered_map<_string, XMFLOAT4X4> m_OffsetMap; // ∫ª ¿Ã∏ß °Ê OffsetMatrix

public:
	static CModel* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		ANIM_TYPE eAnimType,
		const _char* pModelFIlePath,
		FXMMATRIX InitTransformMatrix,
		const _char* pSaveDataName);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END
