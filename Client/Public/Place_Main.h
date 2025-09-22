#pragma once
#include "Place.h"

NS_BEGIN(Client)

class CPlace_Main final : public CPlace
{
private:
	CPlace_Main(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlace_Main() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	static CPlace_Main* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
	
};

NS_END