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

private:
	HRESULT Ready_Items();

private:
	CGameInstance* m_pGameInstance = { nullptr };

private:
	vector<CFoodItem_Texture*> m_pFoodItems;

	uint32_t m_iCurSelectedFoodItemIndex = {};

public:
	virtual void Free()override;
};

NS_END