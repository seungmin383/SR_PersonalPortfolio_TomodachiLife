#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class CComponent;
class CTransformCom;

class ENGINE_DLL CActor abstract : public CObject
{
public:
	typedef struct tagActorDesc : public OBJECT_DESC
	{
		_float fX = {};
		_float fY = {};
		_float fZ = {};

		XMFLOAT4 vPos = {};

		_float fCX = {};
		_float fCY = {};
		_float fCZ = {};

		RENDER_GROUP eRenderGroup;
	}ACTOR_DESC;

protected:
	CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CActor(const CActor& rhs);
	virtual ~CActor() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

protected:
	map	<const _wstring, CComponent*> m_Components;
	CTransformCom* m_pTransformCom = { nullptr };

	_float m_fX = {};
	_float m_fY = {};
	_float m_fZ = {};

	_float m_fCX = {};
	_float m_fCY = {};
	_float m_fCZ = {};

	RENDER_GROUP m_eRenderGroup = { RENDER_GROUP::END };

protected:
	HRESULT Add_Component(
		uint32_t PlaceID,
		const _wstring PrototypeTag,
		const _wstring ComponentTag,
		CComponent** ppOut,
		void* pArg
	);

public:
	virtual void Free()override;
};

NS_END