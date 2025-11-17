#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Game)

class CMiiHead final : public CPartObject
{
private:
	CMiiHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiiHead(const CMiiHead& rhs);
	virtual ~CMiiHead() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	static CMiiHead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END