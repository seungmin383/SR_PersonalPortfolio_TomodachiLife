#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CObject;
class CPrototype final : public CBase
{
private:
	CPrototype(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPrototype() = default;

public:
	HRESULT Add_Prototype(int32_t _ObjectID, CObject* _Origin);
	CObject* Get_Object_Clone(int32_t _ObjectID, void* pArg = nullptr);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	map<int32_t, CObject*>		m_mapOrigin;

public:
	virtual void Free() override;
};

NS_END