#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSEMesh final : public CBase
{
private:
	CSEMesh();
	virtual ~CSEMesh() = default;

public:
	void ReadSEMeshFile(ifstream& In);

private:
	string m_strName = {};
	uint32_t m_iMaterialIndex = {};
	vector<int32_t> m_BoneIndices;
	vector<VTX_TEX_MESH> m_Vertices;
	vector<int32_t> mIndices;


public:
	static CSEMesh* Create();
	virtual void Free()override;
};

NS_END