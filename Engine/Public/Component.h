#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CObject
{
public:
	typedef struct tagComponentDesc : public OBJECT_DESC
	{

	}COMPONENT_DESC;

protected:
	CComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* _pArg)override;

	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

public:
	virtual void Free()override;
};

NS_END