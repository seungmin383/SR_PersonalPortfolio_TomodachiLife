#pragma once
#include "Base.h"
#include "Game_Defines.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END


NS_BEGIN(Game)

class CNarration_TextBox;

class CNarrationManager final : public CBase
{
	DECLARE_SINGLETON(CNarrationManager)

private:
	CNarrationManager();
	virtual ~CNarrationManager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	const _wstring& Get_NarrationText()const { return m_strNarration; }
	void Set_NarrationText(const _wstring& t) { m_strNarration = t; }

	_bool Get_isEnabledNarration()const { return m_isEnableNarration; }
	void Set_isEnabledNarration(_bool is) { m_isEnableNarration = is; }

	const XMFLOAT4& Get_NarrationColor()const { return m_vTextColor; }
	void Set_NarrationColor(const XMFLOAT4& v) { m_vTextColor = v; }

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext;
	CGameInstance* m_pGameInstance = { nullptr };

private:
	_wstring m_strNarration = {};
	_bool m_isEnableNarration = {};
	XMFLOAT4 m_vTextColor = {};

public:
	virtual void Free()override;
};

NS_END