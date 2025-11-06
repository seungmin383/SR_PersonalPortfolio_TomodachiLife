#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CButton;
class CGameInstance;

class CButtonManager final : public CBase
{
private:
	CButtonManager(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);
	virtual ~CButtonManager() = default;

public:
	HRESULT Initialize(uint32_t iNumPlace);
	HRESULT Add_Button(uint32_t iPlaceID, CButton* pButton);

	void All_Deactivate(uint32_t iPlaceID);

private:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	vector<CButton*>* m_Buttons;

	uint32_t m_iNumPlace = {};

public:
	static CButtonManager* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		uint32_t iNumPlace);

	virtual void Free()override;
};

NS_END