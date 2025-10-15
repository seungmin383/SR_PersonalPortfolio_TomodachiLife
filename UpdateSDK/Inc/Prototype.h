#pragma once
#include "Base.h"

#include "TransformCom.h"
#include "ShaderCom.h"
#include "RectBufferCom.h"
#include "TextureCom.h"

NS_BEGIN(Engine)
class CPrototype final : public CBase
{
private:
	CPrototype(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPrototype() = default;

public:
	HRESULT Add_Prototype(
		uint32_t ObjectType,
		uint32_t ObjectID, 
		CObject* Origin);

	CObject* Get_Object_Clone(
		uint32_t ObjectType,
		uint32_t ObjectID, 
		void* pArg = nullptr);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	map<int32_t, CObject*>		m_mapOrigin[static_cast<int32_t>(OBJECT_TYPE::END)];

public:
	static CPrototype* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

NS_END