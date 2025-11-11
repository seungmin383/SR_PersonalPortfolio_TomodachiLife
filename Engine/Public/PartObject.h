#pragma once
#include "Actor.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CActor
{
protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	virtual void Free()override;
};

NS_END	