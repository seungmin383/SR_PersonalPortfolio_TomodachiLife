#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;

NS_END

NS_BEGIN(Game)

class CAnimMacroManager final : public CBase
{
	DECLARE_SINGLETON(CAnimMacroManager)

private:
	CAnimMacroManager();
	virtual ~CAnimMacroManager() = default;

public:
	HRESULT Initialize();

private:
	HRESULT Load_AnimMacros();

private:
	CGameInstance* m_pGameInstance = { nullptr };

	map<_wstring, vector<_wstring>> m_AnimMacros;

public:
	virtual void Free()override;
};

NS_END