#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"
#include "Place.h"

NS_BEGIN(Client)

class CPlace_ShowMii final : public CPlace
{
private:
	CPlace_ShowMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_ShowMii() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Actor_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_Camera_Layer();

	HRESULT Layer_UI_PersonalBase_BackGround();
	HRESULT Layer_UI_PersonalSub_BackGround();

public:
	static CPlace_ShowMii* Create(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END