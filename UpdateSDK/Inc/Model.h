#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CMesh;

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Origin(const _char* pModelFilePath);
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual HRESULT Render()override;
private:
	Assimp::Importer m_Importer = {};
	const aiScene* m_pAIScene = { nullptr };

private:
	uint32_t m_iNumMeshes = {};
	vector<CMesh*> m_Meshes;

private:
	HRESULT Ready_Meshes();

public:
	static CModel* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const _char* pModelFIlePath);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END