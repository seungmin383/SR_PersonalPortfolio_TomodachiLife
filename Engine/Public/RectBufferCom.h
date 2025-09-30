#pragma once

#include"VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRectBufferCom final : public CVIBuffer
{
private:
	CRectBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRectBufferCom(const CRectBufferCom& rhs);
	virtual ~CRectBufferCom() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* _pArg)override;
	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

public:
	static CRectBufferCom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
