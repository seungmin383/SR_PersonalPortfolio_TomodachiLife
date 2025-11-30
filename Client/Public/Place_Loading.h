#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"
#include "Place.h"

NS_BEGIN(Client)

class CLoader;

class CPlace_Loading final : public CPlace
{
private:
	CPlace_Loading(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	virtual ~CPlace_Loading() = default;

public:
	virtual HRESULT Initialize(
		PLACEID NextPlaceID);

	virtual int32_t	 Update(_float fDT) override;
	virtual HRESULT Render_Priority() override;
	virtual HRESULT Render_Late() override;


	HRESULT Ready_UI_Layer();
	HRESULT Ready_Camera_Layer();
private:
	CLoader*				m_pLoader = { nullptr };
	PLACEID		m_eNextPlaceID = { PLACEID::END };

	_float m_fTimer = {};

public:
	static CPlace_Loading* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, 
		PLACEID NextPlaceID);

	virtual void Free() override;
};

NS_END