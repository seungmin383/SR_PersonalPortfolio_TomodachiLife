#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"

#include "Place.h"

NS_BEGIN(Client)

class CPlace_Room final : public CPlace
{
private:
	CPlace_Room(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_Room() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Actor_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_Camera_Layer();

	HRESULT Layer_UI_LevelUp_Inventory();

	HRESULT Layer_Actor_LevelUp_Item();

	HRESULT Layer_UI_FoodItem();

	HRESULT Layer_UI_Category_Button();

	HRESULT Layer_UI_SubCategory_Button();

	HRESULT Layer_UI_PlayerInven_Button();

	HRESULT	Layer_UI_Favorite_Food_Texture();

	HRESULT Layer_UI_LevelBar();

public:
	static CPlace_Room* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

NS_END