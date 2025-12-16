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


	HRESULT Layer_Deactivate(uint32_t iPlaceID, const _wstring& strLayertag);
	HRESULT All_Deactive(uint32_t iPlaceID);

	void Set_CurButton(CButton* pButton);
	CButton* Get_CurButton();

	void Append_Char(wchar_t ch);
	_wstring Get_Text();

public:
	const _bool Get_OnText()const { return m_isOnText; }
	void Set_OnText(_bool is) { m_isOnText = is; }

	void Set_BuildingNameTextureIndex(uint32_t i) { m_iBuildingNameTextureIndex = i; }
	uint32_t Get_BuildingNameTextureIndex() { return m_iBuildingNameTextureIndex; }

	void Set_HasSelectedPlaceMoveButton(_bool isSelected) { m_hasSelectedPlaceMoveButton = isSelected; }
	_bool Get_HasSelectedPlaceMoveButton() const { return m_hasSelectedPlaceMoveButton; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	map<_wstring, vector<CButton*>>* m_Buttons;
	CButton* m_pActivatedButton = { nullptr };
	uint32_t m_iNumPlace = {};

	_bool m_isOnText = { FALSE };

	_wstring m_strBuffer;
	_wstring m_strText = {};
	_bool m_isCommitted = { FALSE };

	/* 건물 이름 텍스쳐 인덱스 */
	uint32_t m_iBuildingNameTextureIndex = {};
	/* 선택된 장소 이동 버튼이 있는지 */
	_bool m_hasSelectedPlaceMoveButton = {};

public:
	static CButtonManager* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		uint32_t iNumPlace);

	virtual void Free()override;
};

NS_END