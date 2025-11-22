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
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

	void Reset_UIManager(uint32_t iInitGroupIndex);
	void Clear_UIManager(uint32_t iInitGroupIndex);
	HRESULT Show_Loading(_float fDT);


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CGameInstance* m_pGameInstance = { nullptr };

	CShaderCom* m_pShader = { nullptr };
	CTextureCom* m_pTexture = { nullptr };
	CRectBufferCom* m_pRectBuffer = { nullptr };

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

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END