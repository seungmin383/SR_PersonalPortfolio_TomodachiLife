#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Game)

class IUnlock abstract : public CBase
{
protected:
	IUnlock();
	virtual ~IUnlock() = default;

public: 
	virtual _bool isUnlock()PURE;

protected:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free()override;
};

/* 옷 가게 해금 조건 */
class CClothStore_UnlockStrategy : public IUnlock
{
private:
	CClothStore_UnlockStrategy():IUnlock(){}
	virtual ~CClothStore_UnlockStrategy() = default;

public:
	virtual _bool isUnlock()override;

public:
	static CClothStore_UnlockStrategy* Create() { return new CClothStore_UnlockStrategy; }
	virtual void Free()override { IUnlock::Free(); }
};

/* 음식점 해금 조건 */
class CFoodStore_UnlockStrategy : public IUnlock
{
private:
	CFoodStore_UnlockStrategy() :IUnlock() {}
	virtual ~CFoodStore_UnlockStrategy() = default;

public:
	virtual _bool isUnlock()override;

public:
	static CFoodStore_UnlockStrategy* Create() { return new CFoodStore_UnlockStrategy; }
	virtual void Free()override { IUnlock::Free(); };
};

NS_END