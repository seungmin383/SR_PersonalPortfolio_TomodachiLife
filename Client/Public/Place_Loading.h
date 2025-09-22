#pragma once
#include "Client_Defines.h"
#include "Place.h"

NS_BEGIN(Client)

class CLoader;
class CPlace_Loading final : public CPlace
{
private:
	CPlace_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlace_Loading() = default;


public:
	virtual HRESULT Initialize(PLACE_ID _eNextLevelID);
	virtual int32_t	 Update(const _float _fDT) override;
	virtual HRESULT Render() override;

private:
	class CLoader* m_pLoader = { nullptr };
	PLACE_ID			m_eNextLevelID = { PLACE_ID::END };

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	HRESULT Ready_Layer_UI();

public:
	static CPlace_Loading* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, PLACE_ID _eNextLevelID);
	virtual void Free() override;
};

NS_END