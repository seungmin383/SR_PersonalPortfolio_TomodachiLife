#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CActor abstract : public CObject
{
public:
	typedef struct tagActorDesc : public OBJECT_DESC
	{

	}ACTOR_DESC;

protected:
	CActor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CActor(const CActor& rhs);
	virtual ~CActor() = default;

public:
	virtual HRESULT Initialize_Origin();
	virtual HRESULT Initialize_Clone(void* _pArg);

	virtual int32_t Update_Priority(const _float _fDT);
	virtual int32_t Update(const _float _fDT);
	virtual void Update_Late(const _float _fDT);
	virtual HRESULT Render();

public:
	virtual void Free()override;
};

NS_END