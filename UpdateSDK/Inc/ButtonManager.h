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
	HRESULT Add_Button(uint32_t iPlaceID,const _wstring& strLayertag,  CButton* pButton);


	HRESULT All_Deactivate(uint32_t iPlaceID, const _wstring& strLayertag);

	void Set_CurButton(CButton* pButton);
	CButton* Get_CurButton();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	map<_wstring, vector<CButton*>>* m_Buttons;
	CButton* m_pActivatedButton = { nullptr };
	uint32_t m_iNumPlace = {};


public:
	static CButtonManager* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		uint32_t iNumPlace);

	virtual void Free()override;
};

NS_END