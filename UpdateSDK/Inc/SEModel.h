#pragma once
#include "Component.h"
#include "SEBone_Data.h"
#include "SEMesh_Data.h"
#include "SEMaterial_Data.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)

class CMesh;
class CSEMaterial;
class CShaderCom;
class CSEBone;
class CAnimation;

class CSEBone;
class CSEMaterial;
class CSEMesh;
class CSEAnimation;

class ENGINE_DLL CSEModel final : public CComponent
{
private:
	CSEModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEModel(const CSEModel& rhs);
	virtual ~CSEModel() = default;

public:
	HRESULT Initialize_SeModel(
		ANIM_TYPE eAnimType,
		const _char* SaveDataName);

	HRESULT Initialize_Clone(void* pArg);
	HRESULT Render_SeModel(uint32_t iMeshIndex);

public:
	HRESULT Load_BoneData();
	HRESULT Load_MeshData();
	HRESULT Load_AnimMeshData();
	HRESULT Load_MaterialData();

	HRESULT Ready_seBone();
	HRESULT Ready_seMesh();
	HRESULT Ready_seMaterial();

	HRESULT Bind_Material(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iMeshIndex,
		SE_TEXTURE_TYPE eMaterialType,
		uint32_t iTextureIndex = 0);

	HRESULT Bind_Bones(
		CShaderCom* pShader,
		const _char* pConstantName,
		uint32_t iMeshIndex);

public:
	const uint32_t Get_NumMesh()const { return m_iNumMesh; }

private:
	_string m_strSaveDataName = {};
	ANIM_TYPE m_eAnimType = { ANIM_TYPE::END };

	uint32_t m_iNumBone = {};
	vector<SE_BONE> m_seBones;

	uint32_t m_iNumMesh = {};
	vector<SE_MESH> m_seMeshes;
	vector<SE_ANIM_MESH> m_seAnimMeshes;

	uint32_t m_iNumMaterial = {};
	vector<SE_MATERIAL> m_seMaterials;

private:
	vector<CSEBone*> m_pBones;
	vector<CSEMesh*> m_pMeshes;
	vector<CSEMaterial*> m_pMaterials;


public:
	static CSEModel* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		ANIM_TYPE eAnimType,
		const _char* SaveDataName);
	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END
