#pragma once
#include "VIBufferCom.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBufferCom
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs); 
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Origin(aiMesh* pAIMesh);
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	static CMesh* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		aiMesh* pAIMesh);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END
