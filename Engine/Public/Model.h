#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CMesh;
class CMaterial;
class CShaderCom;
class CBone;
class CAnimation;

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Origin(
		_string strProtoTag,
		ANIM_TYPE eAnimType,
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

	HRESULT Bind_Bones(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iMeshIndex);

public:
	uint32_t Get_NumMesh()const{ return m_iNumMesh; }
	int32_t Get_BoneIndex(const _char* pBoneName)const;
	void Play_Animation(_float fDT);
	void Set_Animation(uint32_t iAnimIndex, _bool isLoop = FALSE);

private:
	nlohmann::json m_ModelData = {};
	int32_t m_iDataIndex = {};
	_bool m_isFirstInstantiaed = { TRUE };

	Assimp::Importer m_Importer = {};
	const aiScene* m_pAIScene = { nullptr };
	
	uint32_t m_iNumMesh = {};
	vector<CMesh*> m_Meshes;
	XMFLOAT4X4 m_InitMatrix = {};
	ANIM_TYPE m_eAnimType = { ANIM_TYPE::END };

	uint32_t m_iNumMaterial = {};
	vector<CMaterial*> m_Materials;

	// 이 모델의 모든 뼈
	vector<CBone*> m_Bones;
	int32_t m_iBoneIndex = {0};

	uint32_t m_iNumAnimation = {};
	uint32_t m_iCurAnimationIndex = {};

	/* 애니메이션을 어떻게 보관할 것인가 */
	vector<CAnimation* > m_Animations;
	_bool m_isAnimLoop = { FALSE };
	_bool m_isAnimFinished = { FALSE };


private:
	HRESULT Ready_Meshes();
	HRESULT	Ready_Material(const _char* pModelFIlePath);
	HRESULT Ready_Bones_Assimp(
		nlohmann::json& ModelData,
		const aiNode* pAINode,
		int32_t iParentBoneIndex);



	HRESULT Ready_Animations();

	void Ready_Model(
		 nlohmann::json& Data, 
		_string strProtoTag,
		ANIM_TYPE eAnimType);

public:
	static CModel* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		_string strProtoTag,
		ANIM_TYPE eAnimType,
		const _char* pModelFIlePath,
		FXMMATRIX InitTransformMatrix);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END