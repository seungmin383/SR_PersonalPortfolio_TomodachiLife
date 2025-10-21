#pragma once
#include"Place.h"

NS_BEGIN(Client)

class CPlace_Setting final : public CPlace
{
private:
	CPlace_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_Setting() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Setting_Actor_Layer();
	HRESULT Ready_Setting_UI_Layer();
	HRESULT Ready_Setting_Environment_Layer();
	HRESULT Ready_Setting_Camera_Layer();

public:
	static CPlace_Setting* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

NS_END