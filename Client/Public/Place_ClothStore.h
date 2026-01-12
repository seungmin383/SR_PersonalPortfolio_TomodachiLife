#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"
#include "Place.h"

NS_BEGIN(Client)

class CPlace_ClothStore final : public CPlace
{
private:
	CPlace_ClothStore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_ClothStore() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Actor_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_Camera_Layer();

	HRESULT Layer_UI_Bottom_Background();
	HRESULT Layer_UI_Category_Button();
	HRESULT Layer_UI_FoodList_Button();
	HRESULT Layer_UI_BackButton();
	HRESULT Layer_UI_Option_Button();
	HRESULT Layer_UI_Wallet();
	HRESULT Layer_UI_FoodItem();

public:
	static CPlace_ClothStore* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

NS_END