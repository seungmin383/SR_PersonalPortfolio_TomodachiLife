#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"

#include "Place.h"

NS_BEGIN(Client)
class CPlaceMoveButton;

class CPlace_MiiEditor final : public CPlace
{
private:
	CPlace_MiiEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_MiiEditor() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Actor_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Layer_UI_BackGround();
	HRESULT Layer_UI_MiiEdit_Button();
	HRESULT Layer_UI_Page_Button();
	HRESULT Layer_UI_Category_Button();
	HRESULT Layer_UI_Save_Button();
	HRESULT Layer_UI_Text_Button();

	HRESULT Ready_Environment_Layer();
	HRESULT Ready_Camera_Layer();

public:
	static CPlace_MiiEditor* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

NS_END