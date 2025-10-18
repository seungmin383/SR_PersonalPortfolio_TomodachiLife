#pragma once

#include"VIBufferCom.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRectBufferCom final : public CVIBufferCom
{
private:
	CRectBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRectBufferCom(const CRectBufferCom& rhs);
	virtual ~CRectBufferCom() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	static CRectBufferCom* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
