#pragma once

#include"VIBufferCom.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTerrainBufferCom final : public CVIBufferCom
{
private:
	CTerrainBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrainBufferCom(const CTerrainBufferCom& rhs);
	virtual ~CTerrainBufferCom() = default;

public:
	virtual HRESULT Initialize_Origin(const _tchar* HeightMapFilePath);
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	static CTerrainBufferCom* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		const _tchar* HeightMapFilePath);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
