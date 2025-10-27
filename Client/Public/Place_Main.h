#pragma once
#include "Place.h"

NS_BEGIN(Client)

class CPlace_Main final : public CPlace
{
private:
	CPlace_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_Main() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Main_Actor_Layer();
	HRESULT Ready_Main_UI_Layer();
	HRESULT Ready_Main_Environment_Layer();
	HRESULT Ready_Main_Camera_Layer();


public:
	static CPlace_Main* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

NS_END