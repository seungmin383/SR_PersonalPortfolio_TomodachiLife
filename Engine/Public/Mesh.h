#pragma once
#include "VIBufferCom.h"

NS_BEGIN(Engine)
class CShaderCom;
class CBone;
class CModel;

class ENGINE_DLL CMesh final : public CVIBufferCom
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs); 
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Origin(
		nlohmann::json& MeshDataArr,
		int32_t iMeshIndex,
		ANIM_TYPE eAnimType,
		CModel* pModel,
		aiMesh* pAIMesh, 
		FXMMATRIX InitTransformMatrix);

	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	/*
	* 쉐이더에서는 메쉬 단위로, 
	* 메쉬가 사용하는 뼈의 매트릭스를 넘겨줘야됨
	*/
	HRESULT Bind_Bones(
		CShaderCom* pShader,
		const _char* pConstantName,
		vector<CBone*>& Bones);

public:
	uint32_t Get_MaterialIndex() { return m_iMaterialIndex; }

private:
	HRESULT Ready_AnimMesh(
		nlohmann::json& MeshData,
		const aiMesh* pAIMesh,
		CModel* pModel);

	HRESULT Ready_NoneAnimMesh(
		nlohmann::json& MeshData,
		const aiMesh* pAIMesh, 
		FXMMATRIX InitTransforMatrix);

private:
	//모델에서 이 메쉬가 몇번째 메쉬인지
	int32_t m_iMeshindex = {0};

	_char m_szName[MAX_PATH] = {};
	uint32_t m_iMaterialIndex = {};
	uint32_t m_iNumBones = {};

	// 이 메쉬에 영향을 주는 뼈 인덱스
	vector<int32_t > m_BoneIndices;
	// 이 메쉬에 영향을 주는 뼈 매트릭스
	XMFLOAT4X4 m_BoneMatrices[g_iMaxNumBones] = {};

	vector<XMFLOAT4X4>	m_OffsetMatrices;

public:
	static CMesh* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		nlohmann::json& MeshDataArr,
		int32_t iMeshIndex,
		ANIM_TYPE eAnimType,
		CModel* pModel,
		aiMesh* pAIMesh,
		FXMMATRIX InitTransformMatrix);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END
