#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Origin(const _char* pModelFilePath);
	virtual HRESULT Initialize_Clone(void* pArg)override;

private:
	Assimp::Importer m_Importer = {};
	const aiScene* m_pAIScene = { nullptr };

public:
	static CModel* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const _char* pModelFIlePath);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END