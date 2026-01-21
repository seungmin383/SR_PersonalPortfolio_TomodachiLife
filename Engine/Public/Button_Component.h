#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CShaderCom;
class ITrigger_Strategy;

class ENGINE_DLL CButton_Component final : public CComponent
{
private:
	CButton_Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton_Component(const CButton_Component& rhs);
	virtual ~CButton_Component() = default;

public:
	virtual HRESULT Initialize_Origin();
	virtual HRESULT Initialize_Clone(void* pArg)override;

	ITrigger_Strategy* m_pTrigger;

public:
	static CButton_Component* Create(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END