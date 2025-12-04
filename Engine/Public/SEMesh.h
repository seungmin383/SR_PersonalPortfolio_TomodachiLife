#pragma once
#include "VIBufferCom.h"
#include "SEMesh_Data.h"
#include "SEBone_Data.h"

NS_BEGIN(Engine)
class CShaderCom;
class CSEBone;

class ENGINE_DLL CSEMesh final : public CVIBufferCom
{
private:
	CSEMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext );
	virtual ~CSEMesh() = default;

public:
	HRESULT Initialize_Mesh(SE_MESH& seMesh, vector<CSEBone*>& pBones);
	HRESULT Initialize_AnimMesh(SE_ANIM_MESH& seAnimMesh, vector<CSEBone*>& pBones);
	
	HRESULT Bind_Bones(
		CShaderCom* pShader,
		const _char* pConstantName,
		vector<CSEBone*>& pBones);

public:
	const uint32_t Get_MaterialIndex()const { return m_iMaterialIndex; }
	const _string& Get_MeshName()const { return m_strName; }
	const _bool Get_isAlpha() { return m_isAlpha; }
	const _bool Get_isLight() { return m_isLight; }

private:
	string m_strName = {};
	uint32_t m_iMaterialIndex = {};
	vector<int32_t> m_BoneIndices;
	vector<VTX_TEX_MESH> m_Vertices;
	vector<VTX_TEX_ANIM_MESH> m_AnimVertices;
	vector<int32_t> m_Indices;

	vector<XMFLOAT4X4>	m_OffsetMatrices;
	XMFLOAT4X4 m_BoneMatrices[g_iMaxNumBones] = {};

	_bool m_isAlpha = { FALSE };
	_bool m_isLight = { FALSE };

public:
	static CSEMesh* Create_Mesh(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		SE_MESH& seMesh, vector<CSEBone*>& pBones);
	static CSEMesh* Create_AnimMesh(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		SE_ANIM_MESH& seAnimMesh, vector<CSEBone*>& pBones);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END