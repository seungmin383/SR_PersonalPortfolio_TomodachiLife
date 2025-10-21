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
		_float fCX = {};
		_float fCY = {};
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

protected:
	HRESULT Add_Component(
		uint32_t PlaceID,
		const _wstring PrototypeTag,
		const _wstring ComponentTag,
		CComponent** ppOut,
		void* pArg
	);

private:
	_float m_fX = {};
	_float m_fY = {};
	_float m_fCX = {};
	_float m_fCY = {};
public:
	virtual void Free()override;
};

NS_END