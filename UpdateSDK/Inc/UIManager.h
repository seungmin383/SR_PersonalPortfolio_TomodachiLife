#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CUI;
class CUIGroup;

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

class CGameInstance;

class CUIManager final : public CBase
{
private:
	CUIManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUIManager() = default;

public:
	HRESULT Initialize();

public:
	void UIGroup_Update_Priority(_float fDT);
	void UIGroup_Update(_float fDT);
	void UIGroup_Update_Late(_float fDT);

	HRESULT UIGroup_Render();

public:
	HRESULT Add_UI(uint32_t iUIGroupIndex, const _wstring& strUITag, CUI* pUI);
	void Change_UIGroup(uint32_t iNextGroupIndex, _bool isLoading = FALSE);
	void Change_Place(uint32_t iInitGroupIndex);

private:
	void Reset_UIManager(uint32_t iInitGroupIndex);
	void Clear_UIManager(uint32_t iInitGroupIndex);
	_float Calc_FadeForce(_float fDT);

public:
	_float Get_FadeForce();

public:
	_bool Get_isEnable() const { return m_isEnable; }
	_bool Get_isBloodEnable() const { return m_isBloodEnable; }
	_bool Get_isColorEnable() const { return m_isColorEnable; }
	_bool Get_isBirthEnable() const { return m_isBirthEnable; }

	void Set_isEnable(_bool is) { m_isEnable = is; }
	void Set_isBloodEnable(_bool is) { m_isBloodEnable = is; }
	void Set_isColorEnable(_bool is) { m_isColorEnable = is; }
	void Set_isBirthEnable(_bool is) { m_isBirthEnable = is; }

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CGameInstance* m_pGameInstance = { nullptr };

private:
	map <uint32_t, CUIGroup*> m_UIGroups;

	uint32_t m_iCurGroupIndex = {};
	uint32_t m_iNextGroupIndex = {};

	_bool m_isGroupChange = { FALSE };
	_bool m_isPlaceChange = { FALSE };

	_bool m_isLoading = { FALSE };
	_bool m_isLoadingFin = { FALSE };

	_float m_fFadeTimer = {};
	_float m_fFadeForce = {};

private:
	/* mii Editor에서 사용 */
	_bool m_isEnable = {};
	_bool m_isBloodEnable = {};
	_bool m_isColorEnable = {};
	_bool m_isBirthEnable = {};

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END