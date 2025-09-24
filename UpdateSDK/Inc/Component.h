#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CObject
{
public:
	typedef struct tagComponentDesc
	{

	}COMPONENT_DESC;
protected:
	CComponent();
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