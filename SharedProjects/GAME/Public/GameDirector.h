#pragma once
#include "Base.h"
#include "Game_Defines.h"

NS_BEGIN(Engine)
class CGameInstance;

NS_END

NS_BEGIN(Game)

class CGameDirector final : CBase
{
	DECLARE_SINGLETON(CGameDirector)

public:
	enum class GAME_DIRECTION
	{
		NO_EVENT,
		GIVE_ITEM,
		GIVE_FOOD,
		BE_FRIEND,
		END
	};

private:
	CGameDirector();
	virtual ~CGameDirector() = default;

public:
	const GAME_DIRECTION& Get_Direction()const { return m_eDirection; }
	void Set_Direction(const GAME_DIRECTION& dir) { m_eDirection = dir; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	GAME_DIRECTION m_eDirection = { GAME_DIRECTION::NO_EVENT };

public:

	virtual void Free()override;
};

NS_END