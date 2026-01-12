#include "ItemManager.h"
#include "ApartManager.h"
#include "GameInstance.h"
#include "FoodItem_Texture.h"

#include "Engine_FlagHelper.h"

#define ITEM_INFO_DATA_PATH "../../SharedProjects/GAME/GameData/ItemData.json"

IMPLEMENT_SINGLETON(CItemManager)

#define TOFLAG(x) static_cast<int32_t>(CFoodItem_Texture::FOODFLAVOR::x)
#define TOFLAG2(x,y) (TOFLAG(x) | TOFLAG(y))

CItemManager::CItemManager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CItemManager::Initialize()
{
	if (FAILED(Ready_Items()))
		return E_FAIL;

	return S_OK;
}

void CItemManager::Set_ItemIndex(ITEM_TYPE eItemType, uint32_t iIndex)
{
	switch (eItemType)
	{
	case ITEM_TYPE::FOOD:
		m_iCurFoodIndex = iIndex;
		break;
	case ITEM_TYPE::END:
		break;
	default:
		break;
	}
}

HRESULT CItemManager::Ready_Items()
{
	CObject* tmp = { nullptr };

	// Food Item
	{
		_wstring FoodNameArr[]		= {
			TEXT(""),
			TEXT("컵라면"),
			TEXT("숱불구이"),
			TEXT("샌드위치"),
			TEXT("두부김치"),
			TEXT("삼겹살"),
			TEXT("육회"),
			TEXT("장어덮밥"),

			TEXT("계란말이"),
			TEXT("튀김"),
			TEXT("피자"),
			TEXT("스파게티"),
			TEXT("계란후라이"),
			TEXT("어니언링"),
			TEXT("떡볶이"),

			TEXT("붕어빵"),
			TEXT("푸딩"),
			TEXT("군고구마"),
			TEXT("사과사탕"),
			TEXT("바나나"),
			TEXT("참외"),
			TEXT("메론"),

			TEXT("녹즙"),
			TEXT("영양제"),
			TEXT("자몽주스"),
			TEXT("우유"),
			TEXT("물"),
			TEXT("쌍화탕"),
			TEXT("아이스커피"),
		};

		uint32_t FoodPriceArr[]		= {
			0,
			1200,   // 컵라면
			5800,   // 숯불구이
			3000,   // 샌드위치
			4500,   // 두부김치
			5500,   // 삼겹살
			6500,   // 육회
			6800,   // 장어덮밥

			3500,   // 계란말이
			4000,   // 튀김
			5000,   // 피자
			4800,   // 스파게티
			1000,   // 계란후라이
			2500,   // 어니언링
			3000,   // 떡볶이

			800,    // 붕어빵
			1500,   // 푸딩
			1200,   // 군고구마
			2000,   // 사과사탕
			700,    // 바나나
			1500,   // 참외
			2800,   // 메론

			3000,   // 녹즙
			5000,   // 영양제
			2500,   // 자몽주스
			1200,   // 우유
			500,    // 물
			2000,   // 쌍화탕
			1800,   // 아이스커피
		};

		uint32_t FoodQuantityArr[]	= {
			0,   // (None)

			35,  // 컵라면
			90,  // 숯불구이
			45,  // 샌드위치
			70,  // 두부김치
			85,  // 삼겹살
			75,  // 육회
			95,  // 장어덮밥

			50,  // 계란말이
			60,  // 튀김
			80,  // 피자
			75,  // 스파게티
			20,  // 계란후라이
			35,  // 어니언링
			40,  // 떡볶이

			15,  // 붕어빵
			20,  // 푸딩
			25,  // 군고구마
			15,  // 사과사탕
			10,  // 바나나
			25,  // 참외
			35,  // 메론

			30,  // 녹즙
			5,   // 영양제 (포만감 거의 없음)
			25,  // 자몽주스
			20,  // 우유
			10,  // 물
			15,  // 쌍화탕
			15,  // 아이스커피
		};

		int32_t FoodFlavorArr[]		= {
			0, // (None)
			TOFLAG2(SALTY, UMAMI),        // 컵라면
			TOFLAG2(UMAMI, SALTY),        // 숯불구이
			TOFLAG2(SALTY, MILD),         // 샌드위치
			TOFLAG2(SPICY, UMAMI),        // 두부김치
			TOFLAG2(GREASY, UMAMI),       // 삼겹살
			TOFLAG2(UMAMI, MILD),         // 육회
			TOFLAG2(SWEET, UMAMI),        // 장어덮밥

			TOFLAG2(MILD, UMAMI),         // 계란말이
			TOFLAG2(GREASY, SALTY),       // 튀김
			TOFLAG2(SALTY, UMAMI),        // 피자
			TOFLAG2(UMAMI, MILD),         // 스파게티
			TOFLAG2(MILD, UMAMI),         // 계란후라이
			TOFLAG2(GREASY, SALTY),       // 어니언링
			TOFLAG2(SPICY, SWEET),        // 떡볶이

			TOFLAG2(SWEET, MILD),         // 붕어빵
			TOFLAG2(SWEET, MILD),         // 푸딩
			TOFLAG2(SWEET, MILD),         // 군고구마
			TOFLAG2(SWEET, SOUR),         // 사과사탕
			TOFLAG2(SWEET, MILD),         // 바나나
			TOFLAG2(SWEET, REFRESHING),   // 참외
			TOFLAG2(SWEET, REFRESHING),   // 메론

			TOFLAG2(BITTER, REFRESHING),  // 녹즙
			TOFLAG2(BITTER, MILD),        // 영양제
			TOFLAG2(SOUR, REFRESHING),    // 자몽주스
			TOFLAG(MILD),                 // 우유
			TOFLAG(REFRESHING),           // 물
			TOFLAG2(SWEET, BITTER),       // 쌍화탕
			TOFLAG2(BITTER, REFRESHING),  // 아이스커피
		};

		for (uint32_t i = 0; i < 29; i++)
		{
			ITEM_FOOD_INFO Desc = {};
			Desc.strName	= FoodNameArr[i];		/*	 아이템 이름	*/
			Desc.iPrice		= FoodPriceArr[i];		/*		가격		*/
			Desc.iSatiety	= FoodQuantityArr[i];	/*	   포만도		*/
			Desc.iFlavor	= FoodFlavorArr[i];		/*		 맛			*/

			Desc.iTextureIndex = i;
			m_FoodInformations.push_back(Desc);
		}
	}


	/* 옷 아이템 */


	return S_OK;
}

void CItemManager::Free()
{
	Safe_Release(m_pGameInstance);
	CBase::Free();
}
