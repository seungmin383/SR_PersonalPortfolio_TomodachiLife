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
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Origin()	override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

protected:
	_bool m_isClone = { false };

public:
	virtual void Free()override;
};

NS_END