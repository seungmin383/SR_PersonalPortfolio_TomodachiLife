
#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"

#include "Place.h"

NS_BEGIN(Client)

class CPlace_Apart final : public CPlace
{
private:
	CPlace_Apart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_Apart() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Actor_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_Camera_Layer();

public:
	static CPlace_Apart* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

NS_END