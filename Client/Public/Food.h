#pragma once
#include "Client_Defines.h"
#include "Actor.h"


NS_BEGIN(Client)

class CFood final : public CActor
{
protected:
	CFood(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CFood(const CFood& rhs);
	virtual ~CFood() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* _pArg)override;

	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

public:
	static CFood* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END