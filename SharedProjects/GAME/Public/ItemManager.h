#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;

NS_END

NS_BEGIN(Game)

class CFoodItem_Texture;

class CItemManager final : public CBase
{
	DECLARE_SINGLETON(CItemManager)

private:
	CItemManager();
	virtual ~CItemManager() = default;

public:
	HRESULT Initialize();

public:
	void Show_Item(ITEM_TYPE eItemType, uint32_t iIndex);
	uint32_t Get_FoodItemIndex() { return m_iCurSelectedFoodItemIndex; }

	const ITEM_FOOD_INFO& Get_FoodInfo(uint32_t iIndex) const { return m_FoodInformations[iIndex]; }
	const uint32_t Get_NumFood() { return static_cast<uint32_t>(m_FoodInformations.size() - 1); }

private:
	HRESULT Ready_Items();

private:
	CGameInstance* m_pGameInstance = { nullptr };


private:
	vector<ITEM_FOOD_INFO> m_FoodInformations;

	uint32_t m_iCurSelectedFoodItemIndex = {};

public:
	virtual void Free()override;
};

NS_END