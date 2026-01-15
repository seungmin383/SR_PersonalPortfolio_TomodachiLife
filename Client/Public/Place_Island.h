#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"

#include "Place.h"

NS_BEGIN(Game)
class CPlaceMoveButton;
NS_END

NS_BEGIN(Client)

class CPlace_Island final : public CPlace
{
private:
	CPlace_Island(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_Island() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_Actor_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_Camera_Layer();

	HRESULT Ready_Sound();

	HRESULT Ready_Layer_Windows();

private:
	_float m_tmpTime = {};
	XMVECTOR m_tmpTargetPos = {};

	_float m_fTimer = {};

public:
	static CPlace_Island* Create(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END