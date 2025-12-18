#pragma once

namespace Game
{
	typedef struct tagItemFoodInfo
	{
		_wstring strName;
		uint32_t iPrice;
		uint32_t iSatiety;
		int32_t iFlavor;

		uint32_t iTextureIndex;

	}ITEM_FOOD_INFO;

}