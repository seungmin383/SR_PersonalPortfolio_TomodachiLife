#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class CActor abstract : public CObject
{
protected:
	CActor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CActor(const CActor& rhs);
	virtual ~CActor() = default;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free()override;
};

NS_END