#pragma once
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
		Client::PLACE_ID NextPlaceID,
		Engine::RETAIN_POLICY Policy);

	virtual int32_t	 Update(_float fDT) override;
	virtual HRESULT Render_Priority() override;
	virtual HRESULT Render_Late() override;

private:
	CLoader*				m_pLoader = { nullptr };
	Client::PLACE_ID		m_eNextPlaceID = { Client::PLACE_ID::END };
	Engine::RETAIN_POLICY	m_ePolicy = { Engine::RETAIN_POLICY::END };

public:
	static CPlace_Loading* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, 
		Client::PLACE_ID NextPlaceID,
		Engine::RETAIN_POLICY Policy);

	virtual void Free() override;
};

NS_END