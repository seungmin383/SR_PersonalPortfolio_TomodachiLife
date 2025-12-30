#pragma once
#include "Base.h"
#include "Game_Defines.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END


NS_BEGIN(Game)

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
	void Set_NarrationText(const _wstring& t) {	m_strNarration = t;	}

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext;
	CGameInstance* m_pGameInstance = { nullptr };

private:
	_wstring m_strNarration = {};

public:
	virtual void Free()override;
};

NS_END